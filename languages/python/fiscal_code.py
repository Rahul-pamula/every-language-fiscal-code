#!/usr/bin/env python3
"""Italian Fiscal Code (codice fiscale) — Python implementation.

Reads one JSON object from stdin and writes the result to stdout,
following SPECIFICATION.md.
"""

import json
import re
import sys

VOWELS = "AEIOU"
MONTH_CODES = "ABCDEHLMPRST"
NAME_RE = re.compile(r"^[A-Za-z '\-]*\Z")
DATE_RE = re.compile(r"^([0-9]{4})-([0-9]{2})-([0-9]{2})\Z")
BIRTHPLACE_RE = re.compile(r"^[A-Z][0-9]{3}\Z")
REQUIRED_FIELDS = ("surname", "name", "birth_date", "sex", "birthplace_code")

# Values of characters in odd positions (1st, 3rd, ...), indexed by 0-9 then A-Z.
ODD_VALUES = [1, 0, 5, 7, 9, 13, 15, 17, 19, 21,
              1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3,
              6, 8, 12, 14, 16, 10, 22, 25, 24, 23]


class InputError(Exception):
    def __init__(self, code):
        super().__init__(code)
        self.code = code


def normalize_name(value, error_code):
    if not isinstance(value, str) or not NAME_RE.match(value):
        raise InputError(error_code)
    letters = "".join(c for c in value.upper() if "A" <= c <= "Z")
    if not letters:
        raise InputError(error_code)
    return letters


def split_letters(text):
    consonants = [c for c in text if c not in VOWELS]
    vowels = [c for c in text if c in VOWELS]
    return consonants, vowels


def encode_surname(surname):
    consonants, vowels = split_letters(surname)
    return "".join(consonants + vowels + ["X"] * 3)[:3]


def encode_name(name):
    consonants, vowels = split_letters(name)
    if len(consonants) >= 4:
        return consonants[0] + consonants[2] + consonants[3]
    return "".join(consonants + vowels + ["X"] * 3)[:3]


def is_leap_year(year):
    return year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)


def parse_date(value):
    match = DATE_RE.match(value) if isinstance(value, str) else None
    if not match:
        raise InputError("invalid_birth_date")
    year, month, day = (int(part) for part in match.groups())
    days_in_month = [31, 29 if is_leap_year(year) else 28, 31, 30, 31, 30,
                     31, 31, 30, 31, 30, 31]
    if year < 1 or not 1 <= month <= 12 or not 1 <= day <= days_in_month[month - 1]:
        raise InputError("invalid_birth_date")
    return year, month, day


def char_index(c):
    return ord(c) - ord("0") if c.isdigit() else ord(c) - ord("A") + 10


def check_character(partial):
    total = 0
    for position, c in enumerate(partial):
        index = char_index(c)
        if position % 2 == 0:  # odd position, 1-based
            total += ODD_VALUES[index]
        else:
            total += index % 10 if c.isdigit() else index - 10
    return chr(ord("A") + total % 26)


def fiscal_code(data):
    if not isinstance(data, dict):
        raise InputError("invalid_json")
    for field in REQUIRED_FIELDS:
        if field not in data:
            raise InputError("missing_field")

    surname = normalize_name(data["surname"], "invalid_surname")
    name = normalize_name(data["name"], "invalid_name")
    year, month, day = parse_date(data["birth_date"])
    sex = data["sex"]
    if sex not in ("M", "F"):
        raise InputError("invalid_sex")
    birthplace = data["birthplace_code"]
    if not isinstance(birthplace, str) or not BIRTHPLACE_RE.match(birthplace):
        raise InputError("invalid_birthplace_code")

    partial = (encode_surname(surname) + encode_name(name)
               + "%02d" % (year % 100) + MONTH_CODES[month - 1]
               + "%02d" % (day + 40 if sex == "F" else day) + birthplace)
    return partial + check_character(partial)


def main():
    try:
        try:
            data = json.loads(sys.stdin.buffer.read().decode("utf-8"))
        except (UnicodeDecodeError, ValueError):
            raise InputError("invalid_json")
        result = {"fiscal_code": fiscal_code(data)}
    except InputError as error:
        print(json.dumps({"error": error.code}))
        return 1
    print(json.dumps(result))
    return 0


if __name__ == "__main__":
    sys.exit(main())
