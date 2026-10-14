# Contributing

Thank you for helping run the same algorithm in every language!

The unit of contribution is **one language not yet covered**. Check
[LANGUAGES.md](LANGUAGES.md) and the open issues first, then comment on (or
open) the issue for your language so others know you are working on it.

## Adding a language

1. Create `languages/<slug>/`, where `<slug>` is lowercase letters, digits and
   hyphens (`rust`, `common-lisp`, `x86-64-asm`).
2. Implement [SPECIFICATION.md](SPECIFICATION.md): read one JSON object on
   stdin, write one JSON object on stdout, exit 0 or 1.
3. Add a `Dockerfile` that builds or installs everything needed. Compile in the
   image build, not at run time. Requirements:
   - pin the base image to at least a major/minor version tag;
   - the image must provide a `sleep` command (any Debian, Ubuntu or Alpine
     based image does), because the runner keeps one container alive and runs
     every test vector in it with `docker exec`;
   - the program must work without network access.
4. Add a `run.sh` (or any command) that executes the program inside the image.
5. Add a `manifest.yaml`:

   ```yaml
   language: Rust              # display name
   family: general-purpose     # see the families listed below
   implementation: native      # native | adapter
   source:                     # files containing the algorithm
     - src/main.rs
   run: ./run.sh               # command executed in the image's WORKDIR
   version_command: rustc --version   # optional, recorded as evidence
   authors:
     - your-github-handle
   ```

   Families: `general-purpose`, `scripting`, `jvm`, `dotnet`, `functional`,
   `logic`, `historical`, `lisp`, `scientific`, `systems`, `esoteric`,
   `other`.

6. Add a short `README.md` describing the toolchain and anything notable.
7. Run `./elc test <slug>` until every vector passes, then open a Pull Request.

Look at [`languages/python`](languages/python) and
[`languages/php`](languages/php) for complete examples.

## Rules

- **Write the algorithm yourself, in the declared language.** Calling a remote
  API, wrapping another language's executable, using a library that computes
  fiscal codes, or reading precomputed results are not allowed.
- **No special-casing the tests.** Implementations must follow the
  specification, not recognize known inputs.
- **Standard features are fine.** String handling, arrays, arithmetic, regular
  expressions and the standard JSON library of your language are all welcome.
- **Adapters for transport only.** If your language cannot reasonably parse
  JSON (e.g. some historical languages), a small adapter in another language
  may convert JSON to and from a simpler format. It must not contain any of the
  fiscal code logic, and the manifest must say `implementation: adapter` and
  describe it in an `adapter:` field.
- **Status is assigned by CI**, not declared in the manifest. Only
  implementations that pass 100% of the vectors count as `verified`.
- **Synthetic data only.** Never add real people's personal data to tests or
  examples.

## Changing the specification or the test vectors

Open an issue first. Changes to expected outputs affect every implementation
and must be justified against the normative sources cited in
SPECIFICATION.md.

## Code of Conduct

This project follows the [Code of Conduct](CODE_OF_CONDUCT.md).
