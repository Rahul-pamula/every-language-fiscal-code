# Every Language Challenge
## One algorithm, every programming language in the world
### Open-source project promoted by Openapi — Specification and operating plan

> Italian original: [docs/CHALLENGE.it.md](docs/CHALLENGE.it.md)

**Status:** consolidated project proposal  
**Date:** 14 October 2026  
**Promoting organization:** Openapi  
**First implementation:** Italian Fiscal Code  
**Initial goal:** 100 languages  
**Extended goal:** 256+ languages  
**Ambition:** attempt a verifiable Guinness World Record

---

# 1. Vision

Build an open-source collection of independent implementations of the same algorithm in as many programming languages as possible.

The goal is not simply to publish lots of code snippets, but to show that a single computational problem can be solved, executed and verified across languages belonging to profoundly different eras, paradigms and environments.

From COBOL to Rust, from FORTRAN to Python, from Prolog to Assembly, every implementation must follow a common specification and pass the same tests.

The project aims to involve the international developer community, give visibility to rarely used or historical languages, and build a permanent collection of code that actually works.

The project must be useful and interesting even if it never obtains any Guinness recognition.

**Core principle: one algorithm, many languages, a single definition of correctness.**

# 2. Origin of the ideas

## First idea — June 2026

The original proposal was to write the Italian fiscal code algorithm in 100 programming languages, possibly extending the challenge to every language in which an implementation could be built.

The name initially proposed was:

**The Fiscal Code Language Challenge**

Later a more general structure emerged:

**Every Language**

with a first repository named:

`every-language-fiscal-code`

The choice was intentional: start with the fiscal code, but create a format that could host other algorithms in the future.

Slogans discussed:

- "Implementing real-world algorithms in every programming language."
- "A community effort to implement the same algorithm everywhere."

The project was meant to be presented as a collaborative initiative, not as a mere advertising campaign.

## Second idea — October 2026

The goal expanded towards a possible Guinness World Record, with an initiative promoted by Openapi to generate awareness, international attention and developer participation.

The new proposal was called:

**100 Languages Challenge**

A single algorithm, independently implemented in 100 or more languages, with public proof of the correctness of every implementation.

The idea of using SHA-256 — a standardized, sufficiently complex algorithm with public test vectors — was also considered.

The design preference, however, remains to start with the Italian fiscal code, keeping SHA-256 as a possible second challenge or as a more universal alternative for the record attempt.

# 3. Naming and identity

We propose to distinguish the name of the overall project from that of its first challenge.

**Ecosystem:** Every Language  
**Proposed GitHub organization:** `every-language`  
**First repository:** `every-language-fiscal-code`  
**Challenge name:** The Fiscal Code Language Challenge

Public title:

**One Algorithm. Every Programming Language.**

Subtitle:

"An open-source challenge to implement the Italian Fiscal Code algorithm in every programming language."

Openapi appears as promoter and supporter of the initiative, without turning it into a commercial demo of its APIs.

The Guinness name must not appear in the official project name and must not be used in a way that suggests a certification that has not yet been obtained.

# 4. Why the fiscal code

The Italian fiscal code is an interesting problem because it combines several kinds of processing.

It includes:

- string transformations;
- extraction of consonants and vowels;
- character selection rules;
- date transformations;
- encoding of sex and day of birth;
- lookup of territorial codes;
- modular arithmetic;
- computation of a check character;
- handling of *omocodia* (homocode collisions).

It is complex enough to distinguish a real implementation from a mere language demo.

It is also small enough to be implemented in very different languages, including historical and resource-constrained ones.

Moreover, it is a real algorithm with a concrete application in the administrative world.

An essential point is distinguishing the theoretical computation of a fiscal code from its actual assignment and validity at the Italian Tax Registry (*Anagrafe tributaria*). The project must not claim that a generated code is necessarily the one officially assigned to a person.

# 5. Functional specification of the first challenge

The first version covers the ordinary 16-character fiscal code for natural persons.

Canonical input:

```json
{
  "surname": "ROSSI",
  "name": "MARIO",
  "birth_date": "1980-01-01",
  "sex": "M",
  "birthplace_code": "H501"
}
```

Proposed canonical output:

```json
{
  "fiscal_code": "RSSMRA80A01H501U"
}
```

The example above is an illustrative vector, to be verified and included in the official suite only after checking it against a reliable reference implementation.

## Required behaviour

Every implementation must:

1. Accept the same logical data.
2. Apply the same text normalization defined by the specification.
3. Apply the surname encoding rules.
4. Apply the name encoding rules.
5. Encode year, month, day and sex.
6. Use the cadastral code of the municipality or foreign State of birth provided as input.
7. Compute the check character.
8. Return the resulting fiscal code.
9. Handle invalid input according to a common convention.
10. Pass the same conformance suite.

## Later extensions

The second phase may include:

- formal validation of a fiscal code;
- handling of homocode variants;
- decoding of the information contained in the code;
- lookup from municipality and foreign State to territorial code;
- handling of historical place names;
- normalization of names with accents and non-ASCII characters.

Homocode handling must distinguish between generating theoretically admissible variants and the code actually assigned by the authority.

The scope of the first version must stay limited and rigorous, so that the challenge remains feasible even in less modern languages.

# 6. Programming languages

The first milestone is reaching at least 100 valid implementations.

The repository must nevertheless be designed to scale to 256 and beyond, with no artificial limits.

Language families to involve:

| Family | Examples |
|---|---|
| General purpose | C, C++, Rust, Go, Zig, Nim |
| Scripting | Python, PHP, Ruby, Perl, Lua, Tcl |
| JVM | Java, Kotlin, Scala, Groovy |
| .NET | C#, F#, VB.NET |
| Functional | Haskell, OCaml, Erlang, Elixir |
| Logic | Prolog, Mercury |
| Historical | COBOL, FORTRAN, BASIC, Pascal, Ada |
| Lisp | Common Lisp, Scheme, Clojure, Racket |
| Scientific | Julia, R, MATLAB, Octave |
| Systems | x86-64 Assembly, ARM Assembly |
| Others | Smalltalk, Forth, APL, J, D |

The list is indicative and does not constitute the record count.

## What counts as a distinct language?

Public criteria must be established.

Proposal:

- a language with identifiable syntax and semantics;
- an implementation actually written in the declared language;
- execution through an appropriate interpreter, compiler or runtime;
- verifiable behaviour;
- no mere invocation of an implementation written in another language.

Dialects, versions and embedded languages must be classified separately.

For example, C11 and C17 should not automatically count as two distinct languages.

JavaScript and TypeScript must likewise be evaluated according to a documented rule, without automatically assuming that a difference in syntax constitutes an additional language for the purpose of the record.

The classification must be acceptable to an external reviewer as well.

# 7. Repository and organization

Proposed GitHub structure:

```text
every-language-fiscal-code/
├── README.md
├── LICENSE
├── CONTRIBUTING.md
├── CODE_OF_CONDUCT.md
├── SPECIFICATION.md
├── LANGUAGES.md
├── RECORD.md
├── specification/
│   ├── input.schema.json
│   ├── output.schema.json
│   ├── algorithm.md
│   ├── normalization.md
│   └── errors.md
├── test-vectors/
│   ├── basic.json
│   ├── edge-cases.json
│   ├── invalid-inputs.json
│   └── omocodia.json
├── languages/
│   ├── c/
│   ├── cpp/
│   ├── python/
│   ├── php/
│   ├── rust/
│   ├── go/
│   ├── cobol/
│   ├── fortran/
│   ├── prolog/
│   └── ...
├── tools/
│   ├── runner/
│   ├── validator/
│   └── reporter/
├── reports/
├── docs/
└── .github/
    ├── workflows/
    └── ISSUE_TEMPLATE/
```

Each implementation has its own directory.

Example:

```text
languages/rust/
├── manifest.yaml
├── README.md
├── src/
│   └── main.rs
└── run.sh
```

Hypothetical manifest:

```yaml
language: Rust
version: "1.x"
implementation: native
source: src/main.rs
run: ./run.sh
tests: shared
status: verified
```

The `status` field is determined by the verification system, not self-declared by the contributor.

Rust is just an example; the manifest does not impose any particular build system.

# 8. Common interface

Every implementation must be usable by an external process.

We propose a CLI with:

- JSON input on stdin;
- JSON output on stdout;
- diagnostics on stderr;
- consistent exit codes;
- no manual interaction.

Example:

```bash
echo '{
  "surname":"ROSSI",
  "name":"MARIO",
  "birth_date":"1980-01-01",
  "sex":"M",
  "birthplace_code":"H501"
}' | ./run.sh
```

For historical languages or languages where JSON parsing is difficult, an external adapter limited to data transport is admissible.

In those cases, however, the adapter must not implement the fiscal code computation logic.

The manifest must explicitly declare the presence of the adapter.

The computational core must remain in the language that is the subject of the challenge.

# 9. Independence of implementations

This is one of the most important requirements.

Every version must be a real implementation of the algorithm, not a call to an external service or to a library that already performs the whole computation.

The following are not valid as independent implementations:

- a function that calls a remote API to obtain the fiscal code;
- a wrapper around the executable of another language;
- a program that simply reads a precomputed result;
- code that only contains special cases to pass the public tests.

Standard language features, such as string manipulation, arrays, mathematical operations and input reading, are admissible.

Independence of the algorithm must not be confused with an absolute ban on any technical dependency.

The rule must be rigorous enough yet practical.

# 10. Centralized verification system

A common runner makes it possible to execute all implementations and compare their results.

Workflow:

```text
Contributor
    |
    v
Pull Request
    |
    v
Structure check
    |
    v
Runtime preparation
    |
    v
Build / interpretation
    |
    v
Run shared tests
    |
    v
Compare results
    |
    v
Verifiable report
    |
    v
Merge and classification
```

Every language must pass 100% of the mandatory tests.

The runner records:

- language;
- environment version;
- Git commit;
- source hashes;
- operating system and architecture;
- command executed;
- number of tests;
- passed and failed tests;
- exit code;
- overall result.

A language does not automatically become valid just because the contributor opened a Pull Request.

## Implementation states

`proposed` — language requested or announced.

`in_progress` — implementation started.

`submitted` — implementation submitted.

`verified` — implementation passing all mandatory checks.

`rejected` — non-conforming implementation.

`unsupported` — implementation currently not executable on the available infrastructure.

Only the `verified` state contributes to the public count.

The number used for a possible Guinness record must additionally comply with the official criteria that will be communicated for that specific attempt.

# 11. Tests and correctness

The test suite is the same for all languages.

It must include:

- ordinary names and surnames;
- short names;
- short surnames;
- surnames without consonants;
- names with at least four consonants;
- different months;
- boundary dates;
- days of birth and sex;
- valid and invalid cadastral codes;
- check characters;
- malformed input;
- normalization cases defined by the specification.

Tests must be independent of the individual implementations.

The first reference implementation may be written in Python or PHP, but it must not be the only source of truth.

Separately checked reference cases are needed, ideally with two independent implementations.

A public suite plus an additional verification suite is also recommended, to reduce the risk of implementations built only to recognize the known examples.

Real personal data must not be used without a valid reason and an adequate legal basis. Synthetic vectors are preferable.

# 12. Guinness World Records strategy

The project is born with the ambition of obtaining recognition for the largest number of programming languages used to independently implement and verify the same algorithm.

Possible title to propose:

**Most programming languages used to independently implement and execute the same algorithm**

This is a title proposed by the project, not an already recognized Guinness record.

## A fundamental problem

Collecting 100 GitHub folders is not enough to claim a world record.

Projects such as Rosetta Code already collect implementations of common problems in many languages.

The distinguishing factor must therefore be much more precise.

The proposal is to require:

- the same formally specified algorithm;
- independent implementations;
- identical behaviour;
- demonstrable execution;
- a common test suite;
- verified compilation or interpretation;
- a manifest for each language;
- a reproducible public report;
- language classification rules;
- evidence preserved for independent verification.

## Proposed measure

The quantity to measure is:

**The number of distinct languages in which the same algorithm has been implemented and successfully executed according to shared criteria.**

It is not the number of files, lines of code, commits or contributors.

## Preliminary checks

Before presenting the project as an official attempt, we must:

1. Search for any existing Guinness categories.
2. Check whether the proposed title is admissible.
3. Submit an application for a new title, if needed.
4. Obtain the applicable guidelines.
5. Adapt evidence collection to the official guidelines.
6. Plan the formal attempt.

We must not assume the category is approved.

According to the criteria published by Guinness World Records, a record must be measurable, breakable, standardisable and verifiable.

The Italian nature of the fiscal code might make the standardisability assessment harder. For this reason the proposed title must be about the diversity of languages executing the same algorithm, not specifically about the Italian fiscal code.

This approach too will require approval.

The project's public technical demonstration and the official attempt may be separate processes.

It is important to request the official rules before carrying out the formal attempt, not merely before finishing the repository.

# 13. Evidence preservation

The infrastructure must produce reproducible evidence.

For each verified version:

```text
evidence/
  rust/
    manifest.json
    source.sha256
    environment.json
    tests.json
    execution.log
```

The final report must contain:

- identification of the repository version;
- Git commit and tag;
- list of valid implementations;
- results of all tests;
- information about execution tools;
- artifact checksums;
- date of the attempt;
- any attestations from verifiers.

Video recordings, witness statements and other required documents will be collected in accordance with the official instructions, once received.

A GitHub Actions workflow is useful technical evidence, but does not by itself guarantee admissibility for Guinness.

# 14. Community involvement

The project must be designed to grow thanks to external contributors.

The elementary unit of collaboration is:

**A language not yet covered.**

Each contributor can pick a language, implement the algorithm and open a Pull Request.

It should be possible to contribute without knowing Openapi's systems or owning API keys.

## GitHub issues

The repository may generate an issue for each language not yet available.

Examples:

- Implement Italian Fiscal Code in COBOL
- Implement Italian Fiscal Code in Prolog
- Implement Italian Fiscal Code in FORTRAN
- Implement Italian Fiscal Code in Ada

The simplest issues may use `good first issue`, when that genuinely describes the difficulty.

Other labels:

`help wanted`, `language:rust`, `language:cobol`, `historical-language`, `needs-runtime`, `tests-failing`.

Issue distribution may also rely on the open-source contributor portals already evaluated for other Openapi repositories, including Up For Grabs.

## Contributor recognition

Each implementation will include:

- the author's name or handle, if desired;
- a reference to the Pull Request;
- the merge date;
- the language;
- the verification status.

A public page may showcase the community that contributed to the project, without turning contributions into a mandatory individual competition.

# 15. Leaderboard and dashboard

The main page must make progress immediately understandable.

Example:

```text
EVERY LANGUAGE CHALLENGE

One Algorithm. Every Language.

Verified implementations: 42 / 100

Languages proposed:      120
Languages in progress:    18
Contributors:             35
Test suite:               v1.0

Next milestone:           50 languages
```

These numbers are purely illustrative.

The dashboard must show:

- progress towards 100;
- progress towards 256;
- the full list of languages;
- the status of each implementation;
- author or authors;
- test results;
- links to the source;
- information on how to contribute.

A static page generated from the manifests and CI reports is preferable, with no need for a database, backend or complex infrastructure.

# 16. Roadmap

## Phase 0 — Definition and feasibility check

- Formalize the fiscal code algorithm.
- Define the scope of the first version.
- Identify regulatory sources and reference data.
- Establish the rules for distinguishing languages.
- Research precedents and Guinness categories.
- Prepare a title proposal for Guinness.

This phase determines the fundamental requirements.

## Phase 1 — Prototype

Build:

- repository structure;
- technical specification;
- test vectors;
- reference implementation;
- runner;
- first five implementations.

The goal is to show that different languages can share the same tests.

## Phase 2 — Architecture validation

Bring the project to about 10 languages, choosing very different families.

Recommended set:

C, Python, PHP, JavaScript, Rust, Java, COBOL, FORTRAN, Prolog and Haskell.

This phase must above all exercise the hard cases: toolchains, parsing, runtimes, compatibility and output normalization.

## Phase 3 — Public launch

Publish:

- an English README;
- contributor documentation;
- the progress page;
- the first CI results;
- issues for new languages;
- a guide for adding an implementation.

Promote the project in communities dedicated to open source, programming, historical languages and computer science.

The main message must not be "look at our company", but "help us run the same algorithm in every language".

## Phase 4 — Milestones 25, 50 and 100

Growth will be measured through verified implementations.

Each milestone can become an occasion to publish a technical write-up:

- the most unusual languages;
- problems encountered;
- differences between paradigms;
- recovered build tools;
- community contributions.

## Phase 5 — Record attempt

Only after the category has been approved and the official rules received can the Guinness attempt be precisely defined.

The execution and evidence-collection infrastructure will have to be adapted to the required conditions.

## Phase 6 — Expansion

After the 100-language milestone, the project will continue towards 256 and beyond.

Reaching 100 must not be considered a final limit.

# 17. Possible second algorithm: SHA-256

During the October discussion a second direction was proposed.

SHA-256 offers interesting characteristics:

- a standardized algorithm;
- international significance;
- well-established test vectors;
- significant technical complexity;
- no dependency on personal data or territorial codes;
- rigorous comparison between implementations.

This challenge could require a full implementation of SHA-256, forbidding the use of libraries that already perform the hashing.

The trade-off is greater difficulty in historical languages and in environments lacking efficient integer and bit operations.

**Proposed split:**

`every-language-fiscal-code` — first challenge and pilot project.

`every-language-sha256` — possible second challenge, particularly suited to a universal algorithmic demonstration.

There is no need to build both challenges at the same time.

The Guinness attempt could use whichever is most compatible with the approved guidelines.

# 18. Openapi's role

Openapi must be visible as the promoter of the initiative, not as a mandatory intermediary.

The project:

- must not require commercial registration;
- must not depend on Openapi's APIs to run;
- must not contain artificial features created to advertise products;
- must be free to welcome external contributors;
- must have a clear open-source license;
- must remain useful even if the record is not achieved.

Openapi's return comes from associating its brand with an open, technically interesting initiative able to attract the international community.

Openapi may fund or provide:

- repository maintenance;
- CI infrastructure;
- documentation;
- Pull Request reviews;
- contributor coordination;
- project communication;
- possible handling of the Guinness application.

The Openapi brand may appear in the README, on the website and in the documentation as the official promoter.

# 19. Risks and open issues

## 19.1 Record not recognized

There is not yet any official confirmation that the proposed category will be accepted.

The risk must be managed from the start, without promising a Guinness record before approval.

## 19.2 Precedents

Rosetta Code and other multi-language programming projects are important precedents.

We must make sure the challenge offers something objectively distinct and measurable.

## 19.3 Languages that are not truly distinct

The count cannot be artificially inflated through substantially equivalent versions, aliases and dialects.

## 19.4 Non-independent code

Every implementation must contain the actual logic of the algorithm.

Wrappers cannot be considered independent implementations.

## 19.5 Execution in special environments

Some historical languages may require emulators, specialized operating systems or toolchains that are not easily available.

The repository must preserve the instructions and, whenever possible and legally permitted, allow the execution environment to be reproduced.

## 19.6 Maintenance

Implementations may stop compiling after system or compiler updates.

It is necessary to distinguish between historically verified implementations and implementations still verifiable in the current CI.

## 19.7 Regulatory and registry differences

The Italian fiscal code includes rules, exceptions and territorial data that must be handled correctly.

The specification must clarify the exact meaning of "computing", "validating" and "verifying" a fiscal code.

# 20. Non-negotiable principles

The project must follow these rules:

1. The source code of the implementations is public.
2. There is a single behavioural specification for each challenge.
3. All languages pass the same mandatory suite.
4. No implementation is counted without a verified execution.
5. Every language is classified according to transparent criteria.
6. Wrappers delegating the algorithm to another language are not counted.
7. Technical evidence must be reproducible.
8. The number of valid languages is computed automatically.
9. Results are not presented as Guinness-certified without official recognition.
10. Participation must be open and independent of Openapi's commercial services.

# 21. First deliverable

The first concrete result is not a repository with 100 languages.

It is a repository with:

- a stable algorithm specification;
- a shared test suite;
- a centralized runner;
- a standard manifest;
- 5–10 genuinely different implementations;
- a working CI workflow;
- a README and contributor guide;
- an automatically generated JSON report;
- a progress dashboard;
- a document describing the record proposal.

This foundation will later allow the number of languages to grow without changing the architecture.

# 22. Project summary

**Every Language Challenge** is an open-source initiative promoted by Openapi with the goal of implementing the same algorithm in as many programming languages as possible.

The first challenge is the computation of the Italian fiscal code.

Each contributor can build an independent implementation in the language of their choice, following a common specification and passing shared automated tests.

The project starts with a goal of 100 languages, with an architecture able to reach at least 256.

The ambition is to turn the collection into a Guinness World Records attempt based on objective criteria of execution, reproducibility and verification.

The expected result is a public library of working implementations, an international community of contributors and a concrete demonstration of the variety of programming languages.

**One algorithm. Every language. Every implementation verified.**

---

# 23. Instructions for starting local development

Use this document as the initial specification of the project.

Recommended order of work:

1. Create the minimal structure of `every-language-fiscal-code`.
2. Write `SPECIFICATION.md` with the complete rules of the Italian fiscal code, checking them against the regulatory sources.
3. Fix the input/output contract and error handling.
4. Build independent test vectors.
5. Implement the computation in PHP and Python.
6. Build the common runner, preferably as a simple CLI.
7. Add C, Rust and a language from a different paradigm such as Prolog.
8. Integrate CI.
9. Generate a report with valid languages, tests and evidence.
10. Publish the first README for the community.
11. Separately prepare the Guinness category proposal.
12. Expand the challenge through contributors and Pull Requests.

Do not start by automatically generating 100 empty folders or 100 unverified implementations.

The main difficulty is not writing the same algorithm a hundred times: it is building a system in which a hundred different programs can be executed, compared and recognized as authentic implementations.

**Success criterion for the first milestone:** ten genuinely different languages, ten independent implementations, a fully passed shared suite and a single reproducible report.

This is the technical foundation of the whole project.
