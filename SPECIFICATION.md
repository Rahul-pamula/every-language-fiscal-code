# Specification — Italian Fiscal Code

**Specification version:** 0.1.0 (draft)

This document is the single definition of correctness for every implementation
in this repository. An implementation is correct if, and only if, it behaves as
described here and passes every vector in [`test-vectors/`](test-vectors/).

> **Disclaimer.** A fiscal code computed by this algorithm is the *theoretical*
> code for the given data. It is not necessarily the code officially assigned
> by the Italian Revenue Agency (*Agenzia delle Entrate*), which may differ —
> for example because of homocode (*omocodia*) resolution. All test data is
> synthetic.

Normative source: Decreto del Ministro delle Finanze, 23 dicembre 1976,
*"Sistemi di codificazione dei soggetti da iscrivere nell'anagrafe tributaria"*
(D.M. 23/12/1976), and D.P.R. 29 settembre 1973, n. 605.

## 1. Scope of version 0.1

In scope: computing the ordinary 16-character fiscal code of a natural person
from surname, name, date of birth, sex and the cadastral code of the place of
birth.

Out of scope (planned extensions): validating or decoding an existing code,
homocode variants, looking up the cadastral code from a place name, and names
containing non-ASCII letters (e.g. `NICOLÒ`). Inputs belonging to these areas
are either rejected or not tested.

## 2. Interface

An implementation is a program executed by the runner with no arguments.

1. It reads **one JSON document** from standard input, UTF-8 encoded, until
   end of file. The document may contain arbitrary JSON whitespace, including a
   trailing newline, and keys may appear in any order.
2. It writes **one JSON object** to standard output.
3. It exits with code **0** on success and **1** when the input is rejected.
4. It may write anything to standard error; that output is ignored.
5. It must not require interaction, network access (the runner disables the
   network), or any file other than its own sources.

The runner compares output *as JSON*: whitespace and key order are irrelevant.

### 2.1 Input

A JSON object with the following members (see
[`specification/input.schema.json`](specification/input.schema.json)):

| Member            | Type   | Example        |
|-------------------|--------|----------------|
| `surname`         | string | `"ROSSI"`      |
| `name`            | string | `"MARIO"`      |
| `birth_date`      | string | `"1980-01-01"` |
| `sex`             | string | `"M"`          |
| `birthplace_code` | string | `"H501"`       |

Members not listed above must be ignored.

### 2.2 Successful output

```json
{"fiscal_code": "RSSMRA80A01H501U"}
```

### 2.3 Error output

```json
{"error": "invalid_birth_date"}
```

with exit code 1. The error codes are:

| Code                      | Condition |
|---------------------------|-----------|
| `invalid_json`            | Standard input is not valid JSON, or the document is not an object. |
| `missing_field`           | One of the five required members is absent. |
| `invalid_surname`         | `surname` is not a string or violates §3.1. |
| `invalid_name`            | `name` is not a string or violates §3.1. |
| `invalid_birth_date`      | `birth_date` is not a string or violates §3.3. |
| `invalid_sex`             | `sex` is not exactly `"M"` or `"F"`. |
| `invalid_birthplace_code` | `birthplace_code` is not a string matching `^[A-Z][0-9]{3}$`. |

Checks are applied in the order of the table. Every test vector contains a
single defect, so an implementation that validates in a different order still
passes as long as it reports the right code for each defect.

## 3. Algorithm

The fiscal code is the concatenation of seven parts:

```
 RSS   MRA   80   A   01   H501   U
 │     │     │    │   │    │      └─ check character        (1)
 │     │     │    │   │    └──────── birthplace code        (4)
 │     │     │    │   └───────────── day of birth and sex   (2)
 │     │     │    └───────────────── month of birth         (1)
 │     │     └────────────────────── year of birth          (2)
 │     └──────────────────────────── name                   (3)
 └────────────────────────────────── surname                (3)
```

### 3.1 Normalization of surname and name

1. The value may only contain ASCII letters (`A`–`Z`, `a`–`z`), spaces,
   apostrophes (`'`, U+0027) and hyphens (`-`, U+002D). Any other character —
   including digits, dots and non-ASCII letters — makes the input invalid.
2. Letters are converted to upper case.
3. Spaces, apostrophes and hyphens are removed.
4. The result must contain at least one letter; otherwise the input is invalid.

Vowels are `A E I O U`. Every other letter (including `J K W X Y`) is a
consonant.

### 3.2 Surname and name codes

**Surname (3 characters).** Take the consonants of the normalized surname in
order of appearance, followed by its vowels in order of appearance, followed by
`XXX`; keep the first three characters.

| Surname   | Consonants | Vowels | Code  |
|-----------|------------|--------|-------|
| ROSSI     | RSS        | OI     | `RSS` |
| DE LUCA   | DLC        | EUA    | `DLC` |
| ALOE      | L          | AOE    | `LAO` |
| FO        | F          | O      | `FOX` |
| O         | —          | O      | `OXX` |

**Name (3 characters).** If the normalized name has **four or more**
consonants, the code is the 1st, 3rd and 4th consonant. Otherwise apply the
same rule as the surname.

| Name       | Consonants | Code  |
|------------|------------|-------|
| MARIO      | MR         | `MRA` |
| CARLO      | CRL        | `CRL` |
| STEFANO    | STFN       | `SFN` |
| GIAN LUCA  | GNLC       | `GLC` |
| ADA        | D          | `DAA` |
| EO         | —          | `EOX` |

### 3.3 Date of birth

`birth_date` must match `YYYY-MM-DD` exactly (four, two and two ASCII digits)
and denote a valid date of the proleptic Gregorian calendar with a year between
0001 and 9999. A year is a leap year if it is divisible by 4 and not by 100, or
divisible by 400.

- **Year (2 characters):** the last two digits of the year (`1980` → `80`,
  `2009` → `09`, `1900` → `00`).
- **Month (1 character):** from the table below.

  | Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec |
  |-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
  | A   | B   | C   | D   | E   | H   | L   | M   | P   | R   | S   | T   |

- **Day and sex (2 characters):** the day of the month, plus 40 if `sex` is
  `"F"`, written with two digits (`M`, 1st → `01`; `F`, 1st → `41`;
  `F`, 31st → `71`).

### 3.4 Birthplace code

The `birthplace_code` (*codice catastale*) is copied verbatim. It identifies an
Italian municipality (e.g. `H501` Rome, `F205` Milan) or, with prefix `Z`, a
foreign State (e.g. `Z404` United States). In version 0.1 only its format is
checked; it is not looked up in any registry.

### 3.5 Check character

Number the first 15 characters from 1 to 15. Convert each character to a value
with the **odd** table if its position is odd, or the **even** table if it is
even. The check character is the letter whose zero-based alphabet index equals
the sum of all values modulo 26 (`0` → `A`, …, `25` → `Z`).

**Even positions:** digits `0`–`9` have values 0–9; letters `A`–`Z` have
values 0–25 (`A` = 0, `B` = 1, …, `Z` = 25).

**Odd positions:**

| Char | Value | Char | Value | Char | Value | Char | Value |
|------|-------|------|-------|------|-------|------|-------|
| 0 / A | 1  | 5 / F | 13 | K | 2  | U | 16 |
| 1 / B | 0  | 6 / G | 15 | L | 4  | V | 10 |
| 2 / C | 5  | 7 / H | 17 | M | 18 | W | 22 |
| 3 / D | 7  | 8 / I | 19 | N | 20 | X | 25 |
| 4 / E | 9  | 9 / J | 21 | O | 11 | Y | 24 |
|       |    |       |    | P | 3  | Z | 23 |
|       |    |       |    | Q | 6  |   |    |
|       |    |       |    | R | 8  |   |    |
|       |    |       |    | S | 12 |   |    |
|       |    |       |    | T | 14 |   |    |

### 3.6 Worked example

`ROSSI`, `MARIO`, `1980-01-01`, `M`, `H501`:

| Part     | Value  |
|----------|--------|
| Surname  | `RSS`  |
| Name     | `MRA`  |
| Year     | `80`   |
| Month    | `A`    |
| Day/sex  | `01`   |
| Place    | `H501` |

Partial code `RSSMRA80A01H501`. Values:

| Pos | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 |
|-----|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|
| Char | R | S | S | M | R | A | 8 | 0 | A | 0 | 1 | H | 5 | 0 | 1 |
| Table | odd | even | odd | even | odd | even | odd | even | odd | even | odd | even | odd | even | odd |
| Value | 8 | 18 | 12 | 12 | 8 | 0 | 19 | 0 | 1 | 0 | 0 | 7 | 13 | 0 | 0 |

Sum = 98; 98 mod 26 = 20 → `U`. Result: **`RSSMRA80A01H501U`**.

## 4. Test vectors

The shared suite lives in [`test-vectors/`](test-vectors/). Each file has the
form:

```json
{
  "spec_version": "0.1.0",
  "description": "…",
  "cases": [
    {
      "id": "basic-001",
      "description": "…",
      "input": { "surname": "ROSSI", "…": "…" },
      "expected": { "fiscal_code": "RSSMRA80A01H501U" }
    }
  ]
}
```

A case uses `raw_input` (a string sent verbatim) instead of `input` when it
needs to send something that is not a JSON object. Expected exit code is 0
when `expected` contains `fiscal_code`, and 1 when it contains `error`.

The expected values were produced independently by the Python and PHP
implementations and cross-checked against published examples
(`RSSMRA80A01H501U`, `MRTMTT25D09F205Z`).

## 5. Versioning

The specification follows semantic versioning. Any change that alters the
expected output of an existing vector is a major change; adding vectors that
existing correct implementations already pass is a patch change.
