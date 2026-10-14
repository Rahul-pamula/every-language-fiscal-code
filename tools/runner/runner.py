#!/usr/bin/env python3
"""Every Language Challenge — conformance runner.

Builds each implementation's Docker image, feeds it every shared test
vector over stdin and compares the JSON written to stdout with the
expected result. Writes a JSON report plus per-language evidence.

Normally invoked through the ./elc wrapper at the repository root,
which runs this script inside the runner container.
"""

import argparse
import datetime
import hashlib
import json
import os
import re
import shlex
import subprocess
import sys
import uuid
from pathlib import Path

import yaml

RUNNER_VERSION = "0.1.0"
ROOT = Path(os.environ.get("ELC_ROOT", Path(__file__).resolve().parents[2]))
LANGUAGES_DIR = ROOT / "languages"
VECTORS_DIR = ROOT / "test-vectors"
REPORTS_DIR = ROOT / "reports"
SLUG_RE = re.compile(r"^[a-z0-9][a-z0-9-]*$")
FAMILIES = {
    "general-purpose", "scripting", "jvm", "dotnet", "functional", "logic",
    "historical", "lisp", "scientific", "systems", "esoteric", "other",
}
CASE_TIMEOUT = 10
BUILD_TIMEOUT = 1800


class ManifestError(Exception):
    pass


def log(message):
    print(message, file=sys.stderr, flush=True)


def docker(*args, **kwargs):
    kwargs.setdefault("capture_output", True)
    kwargs.setdefault("text", True)
    return subprocess.run(["docker", *args], **kwargs)


def image_name(slug):
    return f"elc/{slug}:local"


# --------------------------------------------------------------------------- #
# Discovery
# --------------------------------------------------------------------------- #

def load_manifest(lang_dir):
    path = lang_dir / "manifest.yaml"
    if not path.is_file():
        raise ManifestError("missing manifest.yaml")
    if not (lang_dir / "Dockerfile").is_file():
        raise ManifestError("missing Dockerfile")
    if not SLUG_RE.match(lang_dir.name):
        raise ManifestError("directory name must match " + SLUG_RE.pattern)
    try:
        manifest = yaml.safe_load(path.read_text(encoding="utf-8"))
    except yaml.YAMLError as error:
        raise ManifestError(f"invalid YAML: {error}")
    if not isinstance(manifest, dict):
        raise ManifestError("manifest must be a mapping")

    for key in ("language", "family", "implementation", "source", "run", "authors"):
        if key not in manifest:
            raise ManifestError(f"missing required key '{key}'")
    if manifest["family"] not in FAMILIES:
        raise ManifestError(f"family must be one of: {', '.join(sorted(FAMILIES))}")
    if manifest["implementation"] not in ("native", "adapter"):
        raise ManifestError("implementation must be 'native' or 'adapter'")
    if manifest["implementation"] == "adapter" and not manifest.get("adapter"):
        raise ManifestError("adapter implementations must describe the adapter in 'adapter'")
    sources = manifest["source"]
    if isinstance(sources, str):
        sources = manifest["source"] = [sources]
    if not sources or not all(isinstance(s, str) for s in sources):
        raise ManifestError("source must be a non-empty list of paths")
    for source in sources:
        if not (lang_dir / source).is_file():
            raise ManifestError(f"source file not found: {source}")
    if not isinstance(manifest["authors"], list) or not manifest["authors"]:
        raise ManifestError("authors must be a non-empty list")
    return manifest


def discover(slugs):
    available = sorted(p.name for p in LANGUAGES_DIR.iterdir() if p.is_dir())
    if not slugs:
        return available
    unknown = [s for s in slugs if s not in available]
    if unknown:
        log(f"Unknown language(s): {', '.join(unknown)}")
        log(f"Available: {', '.join(available)}")
        sys.exit(2)
    return slugs


def load_vectors():
    files = sorted(VECTORS_DIR.glob("*.json"))
    cases, versions, digest = [], set(), hashlib.sha256()
    for path in files:
        data = path.read_bytes()
        digest.update(path.name.encode() + b"\0" + data)
        suite = json.loads(data)
        versions.add(suite["spec_version"])
        for case in suite["cases"]:
            case["suite"] = path.stem
            cases.append(case)
    ids = [c["id"] for c in cases]
    duplicates = sorted({i for i in ids if ids.count(i) > 1})
    if duplicates:
        raise SystemExit(f"Duplicate test vector ids: {', '.join(duplicates)}")
    if len(versions) > 1:
        raise SystemExit(f"Test vector files disagree on spec_version: {sorted(versions)}")
    return {
        "spec_version": versions.pop() if versions else None,
        "files": [p.name for p in files],
        "sha256": digest.hexdigest(),
        "cases": cases,
    }


def source_hashes(lang_dir):
    """sha256 of every file in the implementation directory, sorted by path."""
    lines = []
    for path in sorted(p for p in lang_dir.rglob("*") if p.is_file()):
        rel = path.relative_to(lang_dir).as_posix()
        lines.append(f"{hashlib.sha256(path.read_bytes()).hexdigest()}  {rel}")
    tree = hashlib.sha256("\n".join(lines).encode()).hexdigest()
    return tree, lines


# --------------------------------------------------------------------------- #
# Execution
# --------------------------------------------------------------------------- #

def build(slug, execution_log):
    log(f"[{slug}] building image {image_name(slug)}")
    result = docker("build", "-t", image_name(slug), str(LANGUAGES_DIR / slug),
                    timeout=BUILD_TIMEOUT)
    execution_log.append(f"$ docker build -t {image_name(slug)} languages/{slug}")
    execution_log.append(result.stdout + result.stderr)
    if result.returncode != 0:
        log(result.stdout + result.stderr)
        return None
    inspect = docker("image", "inspect", "--format", "{{.Id}}", image_name(slug))
    return inspect.stdout.strip()


def run_case(container, command, case):
    if "raw_input" in case:
        payload = case["raw_input"]
    else:
        payload = json.dumps(case["input"], ensure_ascii=False)
    try:
        proc = subprocess.run(
            ["docker", "exec", "-i", container, *command],
            input=payload.encode("utf-8"), capture_output=True, timeout=CASE_TIMEOUT,
        )
    except subprocess.TimeoutExpired:
        return {"passed": False, "reason": f"timeout after {CASE_TIMEOUT}s", "payload": payload}

    stdout = proc.stdout.decode("utf-8", errors="replace")
    stderr = proc.stderr.decode("utf-8", errors="replace")
    expected = case["expected"]
    expected_exit = 0 if "fiscal_code" in expected else 1
    outcome = {"exit_code": proc.returncode, "stdout": stdout.strip(), "payload": payload}
    try:
        actual = json.loads(stdout)
    except ValueError:
        actual = None

    if actual != expected:
        outcome.update(passed=False, reason=f"expected {json.dumps(expected)}")
    elif proc.returncode != expected_exit:
        outcome.update(passed=False, reason=f"expected exit code {expected_exit}")
    else:
        outcome["passed"] = True
    if stderr.strip():
        outcome["stderr"] = stderr.strip()[-2000:]
    return outcome


def start_container(slug):
    name = f"elc-{slug}-{uuid.uuid4().hex[:8]}"
    result = docker("run", "-d", "--rm", "--network", "none", "--name", name,
                    "--entrypoint", "sleep", image_name(slug), "infinity")
    if result.returncode != 0:
        log(result.stderr)
        return None
    return name


def environment_version(container, manifest):
    command = manifest.get("version_command")
    if not command:
        return None
    result = docker("exec", container, "sh", "-c", command)
    output = (result.stdout + result.stderr).strip()
    return output.splitlines()[0] if output else None


def evaluate(slug, vectors):
    lang_dir = LANGUAGES_DIR / slug
    entry = {"slug": slug, "path": f"languages/{slug}"}
    execution_log = []

    try:
        manifest = load_manifest(lang_dir)
    except ManifestError as error:
        log(f"[{slug}] invalid manifest: {error}")
        entry.update(status="rejected", reason=f"invalid manifest: {error}")
        return entry, None, execution_log, []

    tree_hash, file_hashes = source_hashes(lang_dir)
    command = shlex.split(manifest["run"])
    entry.update(
        language=manifest["language"],
        family=manifest["family"],
        implementation=manifest["implementation"],
        authors=manifest["authors"],
        source_sha256=tree_hash,
        command=manifest["run"],
    )

    image_id = build(slug, execution_log)
    if image_id is None:
        entry.update(status="rejected", reason="docker build failed")
        return entry, manifest, execution_log, file_hashes
    entry["image_id"] = image_id

    container = start_container(slug)
    if container is None:
        entry.update(status="rejected",
                     reason="could not start container (the image must provide 'sleep')")
        return entry, manifest, execution_log, file_hashes

    results = []
    try:
        entry["environment"] = environment_version(container, manifest)
        log(f"[{slug}] running {len(vectors['cases'])} test vectors"
            + (f" on {entry['environment']}" if entry["environment"] else ""))
        for case in vectors["cases"]:
            outcome = run_case(container, command, case)
            outcome["id"] = case["id"]
            results.append(outcome)
            mark = "PASS" if outcome["passed"] else "FAIL"
            execution_log.append(f"{mark} {case['id']}: {outcome['payload']} -> "
                                 f"exit={outcome.get('exit_code')} {outcome.get('stdout', '')}")
            if not outcome["passed"]:
                log(f"[{slug}]   FAIL {case['id']}: got {outcome.get('stdout')!r} "
                    f"(exit {outcome.get('exit_code')}), {outcome['reason']}")
    finally:
        docker("rm", "-f", container)

    passed = sum(1 for r in results if r["passed"])
    entry["tests"] = {"total": len(results), "passed": passed, "failed": len(results) - passed}
    entry["status"] = "verified" if passed == len(results) and results else "rejected"
    entry["results"] = results
    log(f"[{slug}] {entry['status'].upper()} ({passed}/{len(results)})")
    return entry, manifest, execution_log, file_hashes


def write_evidence(entry, manifest, execution_log, file_hashes, platform):
    directory = REPORTS_DIR / "evidence" / entry["slug"]
    directory.mkdir(parents=True, exist_ok=True)
    (directory / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    (directory / "source.sha256").write_text("\n".join(file_hashes) + "\n")
    (directory / "environment.json").write_text(json.dumps({
        "environment": entry.get("environment"),
        "image": image_name(entry["slug"]),
        "image_id": entry.get("image_id"),
        "platform": platform,
    }, indent=2) + "\n")
    (directory / "tests.json").write_text(json.dumps({
        "status": entry["status"],
        "tests": entry.get("tests"),
        "results": entry.get("results", []),
    }, indent=2, ensure_ascii=False) + "\n")
    (directory / "execution.log").write_text("\n".join(execution_log) + "\n")


def platform_info():
    result = docker("version", "--format", "{{.Server.Os}}|{{.Server.Arch}}|{{.Server.Version}}")
    parts = result.stdout.strip().split("|") if result.returncode == 0 else []
    if len(parts) != 3:
        raise SystemExit("Cannot reach the Docker daemon: " + result.stderr.strip())
    return {"os": parts[0], "arch": parts[1], "docker": parts[2]}


# --------------------------------------------------------------------------- #
# Commands
# --------------------------------------------------------------------------- #

def cmd_test(args):
    vectors = load_vectors()
    platform = platform_info()
    slugs = discover(args.languages)
    log(f"Test suite: spec {vectors['spec_version']}, {len(vectors['cases'])} vectors")

    entries = []
    for slug in slugs:
        entry, manifest, execution_log, file_hashes = evaluate(slug, vectors)
        if manifest is not None:
            write_evidence(entry, manifest, execution_log, file_hashes, platform)
        entry.pop("results", None)
        entries.append(entry)

    verified = [e for e in entries if e["status"] == "verified"]
    report = {
        "runner_version": RUNNER_VERSION,
        "generated_at": datetime.datetime.now(datetime.timezone.utc).isoformat(timespec="seconds"),
        "commit": os.environ.get("GIT_COMMIT") or None,
        "platform": platform,
        "test_suite": {k: v for k, v in vectors.items() if k != "cases"}
                      | {"cases": len(vectors["cases"])},
        "summary": {
            "languages": len(entries),
            "verified": len(verified),
            "rejected": len(entries) - len(verified),
        },
        "languages": entries,
    }
    REPORTS_DIR.mkdir(exist_ok=True)
    (REPORTS_DIR / "report.json").write_text(json.dumps(report, indent=2) + "\n")

    log("")
    log(f"{'LANGUAGE':<20} {'STATUS':<10} TESTS")
    for e in entries:
        tests = e.get("tests")
        score = f"{tests['passed']}/{tests['total']}" if tests else e.get("reason", "")
        log(f"{e.get('language', e['slug']):<20} {e['status']:<10} {score}")
    log(f"\nVerified: {len(verified)}/{len(entries)} — report written to reports/report.json")
    return 0 if len(verified) == len(entries) else 1


def cmd_run(args):
    slug = discover([args.language])[0]
    manifest = load_manifest(LANGUAGES_DIR / slug)
    if build(slug, []) is None:
        return 1
    return subprocess.run(["docker", "run", "--rm", "-i", "--network", "none",
                           image_name(slug), *shlex.split(manifest["run"])]).returncode


def cmd_list(_args):
    for slug in discover([]):
        try:
            manifest = load_manifest(LANGUAGES_DIR / slug)
            print(f"{slug:<20} {manifest['language']:<20} {manifest['family']:<16} "
                  f"{manifest['implementation']}")
        except ManifestError as error:
            print(f"{slug:<20} INVALID MANIFEST: {error}")
    return 0


def main():
    parser = argparse.ArgumentParser(prog="elc", description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="command", required=True)
    test = sub.add_parser("test", help="build and verify implementations (default: all)")
    test.add_argument("languages", nargs="*", metavar="language")
    test.set_defaults(func=cmd_test)
    run = sub.add_parser("run", help="build one implementation and pipe stdin through it")
    run.add_argument("language")
    run.set_defaults(func=cmd_run)
    sub.add_parser("list", help="list implementations").set_defaults(func=cmd_list)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
