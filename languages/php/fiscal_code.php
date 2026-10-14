<?php
/**
 * Italian Fiscal Code (codice fiscale) — PHP implementation.
 *
 * Reads one JSON object from stdin and writes the result to stdout,
 * following SPECIFICATION.md.
 */

declare(strict_types=1);

final class InputError extends Exception
{
}

const MONTH_CODES = 'ABCDEHLMPRST';
const REQUIRED_FIELDS = ['surname', 'name', 'birth_date', 'sex', 'birthplace_code'];
const ODD_VALUES = [
    '0' => 1, '1' => 0, '2' => 5, '3' => 7, '4' => 9, '5' => 13, '6' => 15, '7' => 17, '8' => 19, '9' => 21,
    'A' => 1, 'B' => 0, 'C' => 5, 'D' => 7, 'E' => 9, 'F' => 13, 'G' => 15, 'H' => 17, 'I' => 19, 'J' => 21,
    'K' => 2, 'L' => 4, 'M' => 18, 'N' => 20, 'O' => 11, 'P' => 3, 'Q' => 6, 'R' => 8, 'S' => 12, 'T' => 14,
    'U' => 16, 'V' => 10, 'W' => 22, 'X' => 25, 'Y' => 24, 'Z' => 23,
];

function normalizeName(mixed $value, string $errorCode): string
{
    if (!is_string($value) || preg_match("/^[A-Za-z '\\-]*\\z/", $value) !== 1) {
        throw new InputError($errorCode);
    }
    $letters = preg_replace('/[^A-Z]/', '', strtoupper($value));
    if ($letters === '') {
        throw new InputError($errorCode);
    }
    return $letters;
}

function encodePart(string $text, bool $isName): string
{
    $consonants = preg_replace('/[AEIOU]/', '', $text);
    $vowels = preg_replace('/[^AEIOU]/', '', $text);
    if ($isName && strlen($consonants) >= 4) {
        return $consonants[0] . $consonants[2] . $consonants[3];
    }
    return substr($consonants . $vowels . 'XXX', 0, 3);
}

/** @return array{int, int, int} */
function parseDate(mixed $value): array
{
    if (!is_string($value) || preg_match('/^(\d{4})-(\d{2})-(\d{2})\z/', $value, $m) !== 1) {
        throw new InputError('invalid_birth_date');
    }
    [$year, $month, $day] = [(int) $m[1], (int) $m[2], (int) $m[3]];
    $leap = ($year % 4 === 0 && $year % 100 !== 0) || $year % 400 === 0;
    $daysInMonth = [31, $leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    if ($year < 1 || $month < 1 || $month > 12 || $day < 1 || $day > $daysInMonth[$month - 1]) {
        throw new InputError('invalid_birth_date');
    }
    return [$year, $month, $day];
}

function checkCharacter(string $partial): string
{
    $sum = 0;
    for ($i = 0; $i < strlen($partial); $i++) {
        $c = $partial[$i];
        if ($i % 2 === 0) {
            $sum += ODD_VALUES[$c];
        } else {
            $sum += ctype_digit($c) ? (int) $c : ord($c) - ord('A');
        }
    }
    return chr(ord('A') + $sum % 26);
}

/** @param array<string, mixed> $data */
function fiscalCode(array $data): string
{
    foreach (REQUIRED_FIELDS as $field) {
        if (!array_key_exists($field, $data)) {
            throw new InputError('missing_field');
        }
    }

    $surname = normalizeName($data['surname'], 'invalid_surname');
    $name = normalizeName($data['name'], 'invalid_name');
    [$year, $month, $day] = parseDate($data['birth_date']);
    $sex = $data['sex'];
    if ($sex !== 'M' && $sex !== 'F') {
        throw new InputError('invalid_sex');
    }
    $birthplace = $data['birthplace_code'];
    if (!is_string($birthplace) || preg_match('/^[A-Z]\d{3}\z/', $birthplace) !== 1) {
        throw new InputError('invalid_birthplace_code');
    }

    $partial = encodePart($surname, false)
        . encodePart($name, true)
        . sprintf('%02d', $year % 100)
        . MONTH_CODES[$month - 1]
        . sprintf('%02d', $sex === 'F' ? $day + 40 : $day)
        . $birthplace;
    return $partial . checkCharacter($partial);
}

function main(): int
{
    $raw = stream_get_contents(STDIN);
    try {
        // Decoding into objects first lets us tell "{}" (an object) apart from "[]" (a list).
        $decoded = json_decode((string) $raw, false, 512, JSON_THROW_ON_ERROR);
        if (!$decoded instanceof stdClass) {
            throw new InputError('invalid_json');
        }
        $result = ['fiscal_code' => fiscalCode((array) $decoded)];
    } catch (JsonException) {
        echo json_encode(['error' => 'invalid_json']), "\n";
        return 1;
    } catch (InputError $e) {
        echo json_encode(['error' => $e->getMessage()]), "\n";
        return 1;
    }
    echo json_encode($result), "\n";
    return 0;
}

exit(main());
