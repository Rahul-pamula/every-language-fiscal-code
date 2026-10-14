//! Italian Fiscal Code (codice fiscale) — Rust implementation.
//!
//! Reads one JSON object from stdin and writes the result to stdout,
//! following SPECIFICATION.md. Uses only the standard library, so a small
//! JSON parser is included.

use std::collections::HashMap;
use std::io::{self, Read};
use std::process::ExitCode;

/// A JSON value reduced to what the specification needs.
#[derive(Debug)]
enum Value {
    /// String contents as bytes; non-ASCII characters become bytes >= 0x80.
    Str(Vec<u8>),
    Other,
}

struct Parser<'a> {
    bytes: &'a [u8],
    pos: usize,
}

type ParseResult<T> = Result<T, ()>;

impl<'a> Parser<'a> {
    fn peek(&self) -> Option<u8> {
        self.bytes.get(self.pos).copied()
    }

    fn next(&mut self) -> ParseResult<u8> {
        let byte = self.peek().ok_or(())?;
        self.pos += 1;
        Ok(byte)
    }

    fn expect(&mut self, byte: u8) -> ParseResult<()> {
        if self.next()? == byte { Ok(()) } else { Err(()) }
    }

    fn skip_whitespace(&mut self) {
        while matches!(self.peek(), Some(b' ' | b'\t' | b'\n' | b'\r')) {
            self.pos += 1;
        }
    }

    fn document(&mut self) -> ParseResult<HashMap<Vec<u8>, Value>> {
        self.skip_whitespace();
        if self.peek() != Some(b'{') {
            return Err(());
        }
        let members = self.object()?;
        self.skip_whitespace();
        if self.pos == self.bytes.len() { Ok(members) } else { Err(()) }
    }

    fn value(&mut self) -> ParseResult<Value> {
        self.skip_whitespace();
        match self.peek().ok_or(())? {
            b'"' => Ok(Value::Str(self.string()?)),
            b'{' => self.object().map(|_| Value::Other),
            b'[' => self.array().map(|_| Value::Other),
            b't' => self.literal(b"true"),
            b'f' => self.literal(b"false"),
            b'n' => self.literal(b"null"),
            _ => self.number(),
        }
    }

    fn object(&mut self) -> ParseResult<HashMap<Vec<u8>, Value>> {
        self.expect(b'{')?;
        let mut members = HashMap::new();
        self.skip_whitespace();
        if self.peek() == Some(b'}') {
            self.pos += 1;
            return Ok(members);
        }
        loop {
            self.skip_whitespace();
            if self.peek() != Some(b'"') {
                return Err(());
            }
            let key = self.string()?;
            self.skip_whitespace();
            self.expect(b':')?;
            let value = self.value()?;
            members.insert(key, value);
            self.skip_whitespace();
            match self.next()? {
                b',' => continue,
                b'}' => return Ok(members),
                _ => return Err(()),
            }
        }
    }

    fn array(&mut self) -> ParseResult<()> {
        self.expect(b'[')?;
        self.skip_whitespace();
        if self.peek() == Some(b']') {
            self.pos += 1;
            return Ok(());
        }
        loop {
            self.value()?;
            self.skip_whitespace();
            match self.next()? {
                b',' => continue,
                b']' => return Ok(()),
                _ => return Err(()),
            }
        }
    }

    fn literal(&mut self, word: &[u8]) -> ParseResult<Value> {
        if self.bytes[self.pos..].starts_with(word) {
            self.pos += word.len();
            Ok(Value::Other)
        } else {
            Err(())
        }
    }

    fn digits(&mut self) -> ParseResult<()> {
        if !matches!(self.peek(), Some(b'0'..=b'9')) {
            return Err(());
        }
        while matches!(self.peek(), Some(b'0'..=b'9')) {
            self.pos += 1;
        }
        Ok(())
    }

    fn number(&mut self) -> ParseResult<Value> {
        if self.peek() == Some(b'-') {
            self.pos += 1;
        }
        if self.peek() == Some(b'0') {
            self.pos += 1;
        } else {
            self.digits()?;
        }
        if self.peek() == Some(b'.') {
            self.pos += 1;
            self.digits()?;
        }
        if matches!(self.peek(), Some(b'e' | b'E')) {
            self.pos += 1;
            if matches!(self.peek(), Some(b'+' | b'-')) {
                self.pos += 1;
            }
            self.digits()?;
        }
        Ok(Value::Other)
    }

    fn string(&mut self) -> ParseResult<Vec<u8>> {
        self.expect(b'"')?;
        let mut out = Vec::new();
        loop {
            match self.next()? {
                b'"' => return Ok(out),
                byte if byte < 0x20 => return Err(()),
                b'\\' => match self.next()? {
                    byte @ (b'"' | b'\\' | b'/') => out.push(byte),
                    b'b' => out.push(0x08),
                    b'f' => out.push(0x0c),
                    b'n' => out.push(b'\n'),
                    b'r' => out.push(b'\r'),
                    b't' => out.push(b'\t'),
                    b'u' => {
                        let mut code = 0u32;
                        for _ in 0..4 {
                            let digit = (self.next()? as char).to_digit(16).ok_or(())?;
                            code = code * 16 + digit;
                        }
                        // Non-printable or non-ASCII: invalid in names anyway.
                        out.push(if (0x20..0x80).contains(&code) { code as u8 } else { 0x80 });
                    }
                    _ => return Err(()),
                },
                byte => out.push(byte),
            }
        }
    }
}

const FIELDS: [&str; 5] = ["surname", "name", "birth_date", "sex", "birthplace_code"];
const MONTH_CODES: &[u8; 12] = b"ABCDEHLMPRST";
const ODD_VALUES: [u32; 36] = [
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21, //
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3, //
    6, 8, 12, 14, 16, 10, 22, 25, 24, 23,
];

fn string_field<'v>(value: &'v Value, error: &'static str) -> Result<&'v [u8], &'static str> {
    match value {
        Value::Str(bytes) => Ok(bytes),
        Value::Other => Err(error),
    }
}

fn normalize_name(value: &Value, error: &'static str) -> Result<Vec<u8>, &'static str> {
    let mut letters = Vec::new();
    for &byte in string_field(value, error)? {
        match byte {
            b'A'..=b'Z' | b'a'..=b'z' => letters.push(byte.to_ascii_uppercase()),
            b' ' | b'\'' | b'-' => {}
            _ => return Err(error),
        }
    }
    if letters.is_empty() { Err(error) } else { Ok(letters) }
}

fn is_vowel(byte: u8) -> bool {
    b"AEIOU".contains(&byte)
}

fn encode_part(text: &[u8], is_name: bool) -> String {
    let (vowels, consonants): (Vec<u8>, Vec<u8>) = text.iter().partition(|&&b| is_vowel(b));
    let chosen: Vec<u8> = if is_name && consonants.len() >= 4 {
        vec![consonants[0], consonants[2], consonants[3]]
    } else {
        consonants.iter().chain(&vowels).chain(b"XXX").take(3).copied().collect()
    };
    String::from_utf8(chosen).expect("ASCII letters")
}

fn parse_date(value: &Value) -> Result<(u32, u32, u32), &'static str> {
    const ERROR: &str = "invalid_birth_date";
    let s = string_field(value, ERROR)?;
    let well_formed = s.len() == 10
        && s.iter().enumerate().all(|(k, b)| if k == 4 || k == 7 { *b == b'-' } else { b.is_ascii_digit() });
    if !well_formed {
        return Err(ERROR);
    }
    let number = |range: std::ops::Range<usize>| {
        s[range].iter().fold(0u32, |acc, b| acc * 10 + u32::from(b - b'0'))
    };
    let (year, month, day) = (number(0..4), number(5..7), number(8..10));
    let leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    let days_in_month = match month {
        1 | 3 | 5 | 7 | 8 | 10 | 12 => 31,
        4 | 6 | 9 | 11 => 30,
        2 if leap => 29,
        2 => 28,
        _ => return Err(ERROR),
    };
    if year < 1 || day < 1 || day > days_in_month {
        return Err(ERROR);
    }
    Ok((year, month, day))
}

fn check_character(partial: &str) -> char {
    let sum: u32 = partial
        .bytes()
        .enumerate()
        .map(|(k, b)| {
            let index = if b.is_ascii_digit() { b - b'0' } else { b - b'A' + 10 } as usize;
            if k % 2 == 0 {
                ODD_VALUES[index]
            } else if b.is_ascii_digit() {
                u32::from(b - b'0')
            } else {
                u32::from(b - b'A')
            }
        })
        .sum();
    char::from(b'A' + (sum % 26) as u8)
}

fn fiscal_code(input: &HashMap<Vec<u8>, Value>) -> Result<String, &'static str> {
    let field = |name: &str| input.get(name.as_bytes());
    if FIELDS.iter().any(|name| field(name).is_none()) {
        return Err("missing_field");
    }
    let surname = normalize_name(field("surname").unwrap(), "invalid_surname")?;
    let name = normalize_name(field("name").unwrap(), "invalid_name")?;
    let (year, month, day) = parse_date(field("birth_date").unwrap())?;
    let sex = match field("sex").unwrap() {
        Value::Str(s) if s == b"M" || s == b"F" => s[0],
        _ => return Err("invalid_sex"),
    };
    let place = match field("birthplace_code").unwrap() {
        Value::Str(s)
            if s.len() == 4 && s[0].is_ascii_uppercase() && s[1..].iter().all(u8::is_ascii_digit) =>
        {
            String::from_utf8(s.clone()).expect("ASCII")
        }
        _ => return Err("invalid_birthplace_code"),
    };

    let partial = format!(
        "{}{}{:02}{}{:02}{}",
        encode_part(&surname, false),
        encode_part(&name, true),
        year % 100,
        char::from(MONTH_CODES[month as usize - 1]),
        if sex == b'F' { day + 40 } else { day },
        place
    );
    let check = check_character(&partial);
    Ok(format!("{partial}{check}"))
}

fn main() -> ExitCode {
    let mut input = Vec::new();
    let result = match io::stdin().read_to_end(&mut input) {
        Ok(_) => Parser { bytes: &input, pos: 0 }
            .document()
            .map_err(|()| "invalid_json")
            .and_then(|members| fiscal_code(&members)),
        Err(_) => Err("invalid_json"),
    };
    match result {
        Ok(code) => {
            println!("{{\"fiscal_code\":\"{code}\"}}");
            ExitCode::SUCCESS
        }
        Err(error) => {
            println!("{{\"error\":\"{error}\"}}");
            ExitCode::from(1)
        }
    }
}
