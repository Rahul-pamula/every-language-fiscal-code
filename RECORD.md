# Record proposal

> **Status: not submitted.** This project has **not** been recognized by
> Guinness World Records, and no category has been approved. Nothing in this
> repository should be read as a certification.

## Proposed title

**Most programming languages used to independently implement and execute the
same algorithm.**

The title is deliberately about the diversity of languages executing a single
algorithm, not about the Italian fiscal code specifically, to keep it
standardisable and breakable by others.

## Measure

The number of **distinct languages** (see [LANGUAGES.md](LANGUAGES.md)) in
which the same formally specified algorithm has been implemented and
**successfully executed** against a shared test suite.

It is not the number of files, lines of code, commits or contributors.

## What distinguishes this from existing collections

Collections such as Rosetta Code show many languages solving common tasks.
This project additionally requires:

- one formal specification ([SPECIFICATION.md](SPECIFICATION.md));
- independent implementations, with transport adapters declared explicitly;
- identical observable behaviour on a common suite of test vectors;
- automated, reproducible execution of every implementation in a pinned
  container image;
- a manifest and source hashes for every implementation;
- a public report and preserved evidence for each run.

## Evidence produced today

Every `./elc test` run writes `reports/report.json` and, for each language,
`reports/evidence/<language>/` containing the manifest, source hashes,
environment, per-vector results and the execution log. CI publishes these as
artifacts for every commit.

A CI run is useful technical evidence but does not by itself satisfy any
official guidelines.

## Steps before an official attempt

1. Search for existing Guinness categories.
2. Check whether the proposed title is admissible.
3. Apply for a new title, if needed.
4. Obtain the official guidelines.
5. Adapt evidence collection to those guidelines.
6. Plan the formal attempt.

The official rules must be obtained before the formal attempt is carried out.
See section 12 of [CHALLENGE.md](CHALLENGE.md) for the full rationale.
