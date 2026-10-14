#!/usr/bin/env node
/**
 * Italian Fiscal Code (codice fiscale) — JavaScript (Node.js) implementation.
 *
 * Reads one JSON object from stdin and writes the result to stdout,
 * following SPECIFICATION.md.
 */
'use strict';

const fs = require('fs');

const MONTH_CODES = 'ABCDEHLMPRST';
const FIELDS = ['surname', 'name', 'birth_date', 'sex', 'birthplace_code'];
const ODD_VALUES = {
  0: 1, 1: 0, 2: 5, 3: 7, 4: 9, 5: 13, 6: 15, 7: 17, 8: 19, 9: 21,
  A: 1, B: 0, C: 5, D: 7, E: 9, F: 13, G: 15, H: 17, I: 19, J: 21,
  K: 2, L: 4, M: 18, N: 20, O: 11, P: 3, Q: 6, R: 8, S: 12, T: 14,
  U: 16, V: 10, W: 22, X: 25, Y: 24, Z: 23,
};

class InputError extends Error {}

function normalizeName(value, code) {
  if (typeof value !== 'string' || !/^[A-Za-z '-]*$/.test(value)) throw new InputError(code);
  const letters = value.toUpperCase().replace(/[^A-Z]/g, '');
  if (letters === '') throw new InputError(code);
  return letters;
}

function encodePart(text, isName) {
  const consonants = text.replace(/[AEIOU]/g, '');
  const vowels = text.replace(/[^AEIOU]/g, '');
  if (isName && consonants.length >= 4) return consonants[0] + consonants[2] + consonants[3];
  return (consonants + vowels + 'XXX').slice(0, 3);
}

function parseDate(value) {
  const match = typeof value === 'string' && /^(\d{4})-(\d{2})-(\d{2})$/.exec(value);
  if (!match) throw new InputError('invalid_birth_date');
  const [year, month, day] = match.slice(1).map(Number);
  const leap = (year % 4 === 0 && year % 100 !== 0) || year % 400 === 0;
  const daysInMonth = [31, leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  if (year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth[month - 1]) {
    throw new InputError('invalid_birth_date');
  }
  return { year, month, day };
}

function checkCharacter(partial) {
  let sum = 0;
  [...partial].forEach((c, i) => {
    if (i % 2 === 0) sum += ODD_VALUES[c];
    else sum += /\d/.test(c) ? Number(c) : c.charCodeAt(0) - 65;
  });
  return String.fromCharCode(65 + (sum % 26));
}

const pad2 = (n) => String(n).padStart(2, '0');

function fiscalCode(input) {
  if (input === null || typeof input !== 'object' || Array.isArray(input)) {
    throw new InputError('invalid_json');
  }
  if (!FIELDS.every((field) => Object.hasOwn(input, field))) throw new InputError('missing_field');

  const surname = normalizeName(input.surname, 'invalid_surname');
  const name = normalizeName(input.name, 'invalid_name');
  const { year, month, day } = parseDate(input.birth_date);
  if (input.sex !== 'M' && input.sex !== 'F') throw new InputError('invalid_sex');
  const place = input.birthplace_code;
  if (typeof place !== 'string' || !/^[A-Z]\d{3}$/.test(place)) {
    throw new InputError('invalid_birthplace_code');
  }

  const partial = encodePart(surname, false) + encodePart(name, true) + pad2(year % 100)
    + MONTH_CODES[month - 1] + pad2(input.sex === 'F' ? day + 40 : day) + place;
  return partial + checkCharacter(partial);
}

function main() {
  try {
    let input;
    try {
      input = JSON.parse(fs.readFileSync(0, 'utf8'));
    } catch {
      throw new InputError('invalid_json');
    }
    console.log(JSON.stringify({ fiscal_code: fiscalCode(input) }));
    return 0;
  } catch (error) {
    if (!(error instanceof InputError)) throw error;
    console.log(JSON.stringify({ error: error.message }));
    return 1;
  }
}

process.exitCode = main();
