# 🛣️ Roadmap

**One algorithm, every language, step by step.**

This is a long-horizon project. We grow by milestones, not deadlines: each
milestone is a number of **verified** languages, and each one unlocks the
infrastructure the next one needs. We move on when a milestone is reached,
not when a date arrives.

```text
  ✅ ──── 64 ──── 99 ──── 128 ──── 256 ──── 512 ──── 1024
 base     2⁶     🍺      2⁷       2⁸       2⁹       2¹⁰
          ▲
       we are here
```

| Milestone | Why this number | What it unlocks |
|---|---|---|
| ✅ **Foundation** | — | Spec, test vectors, runner, CI, first languages |
| 🎯 **64** | 2⁶, the number of computing | Built by the core team, 10 languages per session; spec 1.0 |
| 🍺 **99** | *99 Bottles of Beer* | Community takes over; dashboard, hidden suite, Guinness application |
| 🔤 **128** | 2⁷, the ASCII table | Historical machines and emulators, spec extensions |
| 💾 **256** | 2⁸, one byte | Formal record attempt, second challenge |
| 🧮 **512** | 2⁹ | Rare and esoteric languages, long-term archive |
| 🚀 **1024** | 2¹⁰, one "kibi" of languages | The moonshot: every language that can run it |

Live progress: [README](README.md) · open work:
[milestones](https://github.com/openapi/every-language-fiscal-code/milestones).

---

## ✅ Foundation (done)

- [x] Translated and published the [challenge](CHALLENGE.md)
- [x] [Specification](SPECIFICATION.md) v0.1.0: algorithm, I/O contract, error codes
- [x] 87 shared [test vectors](test-vectors/), cross-checked by two independent implementations
- [x] Dockerized runner (`./elc`), per-language containers, network disabled
- [x] JSON report and per-language evidence
- [x] GitHub Actions CI
- [x] Python and PHP verified
- [x] Hacktoberfest 2026, with one issue per wanted language

## 🎯 64: the first goal

*2⁶. The word size of every modern processor; see [Why 64?](README.md#-why-64).*

The first 64 languages are built **by the core team**, at a steady pace of
**ten languages per working session**. That gives the community a solid,
varied base to start from, and proves that the runner and the specification
hold up across very different paradigms before anyone else depends on them.
Community Pull Requests are welcome at any time and simply make us faster.

**Sessions** (languages already verified are ticked):

| Session | Languages | Total |
|---|---|---|
| 1 ✅ | ✅ Python, ✅ PHP, ✅ C, ✅ C++, ✅ Rust, ✅ Go, ✅ JavaScript, ✅ Java, ✅ Ruby, ✅ Perl | 10 |
| 2 | C#, Kotlin, Scala, Lua, Swift, Dart, Haskell, OCaml, Elixir, Erlang | 20 |
| 3 | F#, Clojure, Common Lisp, Scheme, Racket, Prolog, Julia, R, Tcl, Groovy | 30 |
| 4 | Bash, AWK, COBOL, FORTRAN, Ada, Pascal, D, Nim, Zig, Crystal | 40 |
| 5 | VB.NET, Octave, Smalltalk, Forth, Raku, PowerShell, Gleam, V, Odin, Standard ML | 50 |
| 6 | Idris, Mercury, Haxe, Vala, Objective-C, FreeBASIC, x86-64 Assembly, Io, Rexx, Chapel | 60 |
| 7 | APL, J, Pony, Hare | **64** |

The order may change if a toolchain turns out to be hard to containerize. A
language that cannot be verified in its session moves to a later one instead
of blocking the others.

**Infrastructure, built along the way.**
- [ ] CI runs only the languages touched by a Pull Request, plus all of them nightly
- [ ] Languages run in parallel in CI
- [ ] Badge, progress bars and [LANGUAGES.md](LANGUAGES.md) generated from `report.json`
- [ ] Contributor credits generated from manifests

**Specification.**
- [ ] Freeze **spec 1.0**: no more changes to existing expected outputs
- [ ] Publish the language-classification rules as final (versions, dialects, supersets)

**Community.**
- [ ] Write-up: "The first 64 languages"
- [ ] Open the issue backlog for the next milestones

## 🍺 99: a toast to 99 Bottles

*The [99 Bottles of Beer](https://www.99-bottles-of-beer.net) project has
collected the same little song in well over a thousand languages since the
1990s, a classic of polyglot programming. At 99 we raise a glass to it, one
step before the symbolic 100.*

From here on the **community** leads: every language beyond the core set is
an open issue waiting for a contributor.

**Infrastructure.**
- [ ] Static dashboard on GitHub Pages: progress, languages, authors, tests, sources
- [ ] Hidden verification suite, run by CI alongside the public one, so that
      implementations cannot be tuned to known examples
- [ ] Tagged releases with a signed evidence bundle (report, hashes, logs)

**Record.**
- [ ] Search for existing Guinness World Records categories
- [ ] Submit the title proposal from [RECORD.md](RECORD.md)
- [ ] Obtain the official guidelines and adapt evidence collection to them

**Community.**
- [ ] Write-up: paradigms, toolchains, surprises

## 🔤 128: the ASCII milestone

*2⁷. The 128 characters of ASCII, the table that taught computers to write.*

**Historical computing.**
- [ ] Support for languages that need emulators or vintage environments
      (e.g. DOS, mainframe or 8-bit toolchains in containers)
- [ ] Distinguish **historically verified** implementations from those
      **verifiable in current CI**
- [ ] Toolchain recipes preserved in the repository whenever legally possible

**Specification 1.x extensions.**
- [ ] Validate an existing fiscal code
- [ ] Decode the information contained in a code
- [ ] Homocode (*omocodia*) variants

## 💾 256: one byte of languages

*2⁸. Every value a byte can hold: one language for each.*

**Record.**
- [ ] Formal record attempt, **only** if the category has been approved and
      under the official rules received

**Ecosystem.**
- [ ] Launch the second challenge, `every-language-sha256`, on the same runner
- [ ] Multi-architecture verification (amd64 and arm64)

## 🧮 512: the long tail

*2⁹. Rare, academic and esoteric languages.*

- [ ] Dedicated track for esoteric languages, with classification rules
- [ ] Long-term archive of sources and toolchain images (e.g. Software Heritage)
- [ ] Reproducible rebuilds of every verified implementation from a tag

## 🚀 1024: the moonshot

*2¹⁰. One "kibi" of languages.*

Every programming language that can still be run, running the same algorithm
and passing the same tests. We may never get there, and that is fine: the
journey is the project.

---

## How the roadmap evolves

- A milestone is **reached** when the number of `verified` languages in the CI
  report on `main` reaches it.
- Infrastructure items may land earlier than their milestone. They are listed
  where they become **necessary**.
- Changes to this roadmap are proposed through issues or Pull Requests.
