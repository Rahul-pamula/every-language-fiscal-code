# Every Language — Italian Fiscal Code

**One Algorithm. Every Programming Language.**

An open-source challenge to implement the Italian Fiscal Code algorithm in
every programming language — each implementation independent, each one
verified by the same shared test suite.

Promoted by [Openapi](https://openapi.com). No API keys, accounts or
commercial services are needed to take part.

| Milestone | Verified languages |
|-----------|--------------------|
| First     | 2 / 10             |
| Goal      | 2 / 100            |
| Extended  | 2 / 256            |

Verified implementations: **Python**, **PHP**. See [LANGUAGES.md](LANGUAGES.md).

## How it works

- [SPECIFICATION.md](SPECIFICATION.md) defines the algorithm and the
  stdin/stdout JSON contract — the single definition of correctness.
- [`test-vectors/`](test-vectors/) holds the shared conformance suite.
- [`languages/<name>/`](languages/) holds one implementation per language, each
  with its own `Dockerfile`.
- The runner builds every implementation in its container, feeds it every test
  vector and writes a reproducible report with source hashes and evidence.

```console
$ echo '{"surname":"ROSSI","name":"MARIO","birth_date":"1980-01-01","sex":"M","birthplace_code":"H501"}' | ./elc run python
{"fiscal_code": "RSSMRA80A01H501U"}
```

## Quick start

The only requirements are **Docker** and **bash**. Every toolchain — including
the runner itself — runs in a container, so nothing gets installed on your
machine.

```bash
./elc list              # list implementations
./elc test              # build and verify every implementation
./elc test python php   # verify only some of them
./elc run php < in.json # pipe input through one implementation
./elc shell python      # open a shell inside a language image
```

Results are written to `reports/report.json` and `reports/evidence/`.

## Contributing

The unit of contribution is **one language not yet covered**. Pick one, read
[CONTRIBUTING.md](CONTRIBUTING.md), make `./elc test <language>` pass and open
a Pull Request.

## Background

- [CHALLENGE.md](CHALLENGE.md) — project vision and operating plan
  ([Italian original](docs/CHALLENGE.it.md))
- [RECORD.md](RECORD.md) — the proposed world record attempt
- [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md)

A fiscal code computed here is the theoretical code for the given data, not
necessarily the one officially assigned by the Italian Revenue Agency. All test
data is synthetic.

## License

[MIT](LICENSE)
