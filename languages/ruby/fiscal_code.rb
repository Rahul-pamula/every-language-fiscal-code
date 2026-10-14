#!/usr/bin/env ruby
# frozen_string_literal: true

# Italian Fiscal Code (codice fiscale) — Ruby implementation.
#
# Reads one JSON object from stdin and writes the result to stdout,
# following SPECIFICATION.md.

require 'json'

class InputError < StandardError; end

MONTH_CODES = 'ABCDEHLMPRST'
FIELDS = %w[surname name birth_date sex birthplace_code].freeze
ODD_VALUES = ('0'..'9').to_a.concat(('A'..'Z').to_a).zip(
  [1, 0, 5, 7, 9, 13, 15, 17, 19, 21,
   1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3,
   6, 8, 12, 14, 16, 10, 22, 25, 24, 23]
).to_h.freeze

def normalize_name(value, code)
  raise InputError, code unless value.is_a?(String) && value.match?(/\A[A-Za-z '\-]*\z/)

  letters = value.upcase.delete('^A-Z')
  raise InputError, code if letters.empty?

  letters
end

def encode_part(text, name:)
  consonants = text.delete('AEIOU')
  vowels = text.delete('^AEIOU')
  return consonants[0] + consonants[2] + consonants[3] if name && consonants.length >= 4

  (consonants + vowels + 'XXX')[0, 3]
end

def leap_year?(year)
  (year % 4).zero? && (year % 100 != 0 || (year % 400).zero?)
end

def parse_date(value)
  match = value.is_a?(String) && value.match(/\A(\d{4})-(\d{2})-(\d{2})\z/)
  raise InputError, 'invalid_birth_date' unless match

  year, month, day = match.captures.map(&:to_i)
  days_in_month = [31, leap_year?(year) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
  unless year >= 1 && (1..12).cover?(month) && (1..days_in_month[month - 1]).cover?(day)
    raise InputError, 'invalid_birth_date'
  end

  [year, month, day]
end

def check_character(partial)
  sum = partial.each_char.with_index.sum do |c, i|
    if i.even? then ODD_VALUES.fetch(c)
    elsif c.match?(/\d/) then c.to_i
    else c.ord - 'A'.ord
    end
  end
  ('A'.ord + (sum % 26)).chr
end

def fiscal_code(input)
  raise InputError, 'invalid_json' unless input.is_a?(Hash)
  raise InputError, 'missing_field' unless FIELDS.all? { |field| input.key?(field) }

  surname = normalize_name(input['surname'], 'invalid_surname')
  name = normalize_name(input['name'], 'invalid_name')
  year, month, day = parse_date(input['birth_date'])
  sex = input['sex']
  raise InputError, 'invalid_sex' unless %w[M F].include?(sex)

  place = input['birthplace_code']
  unless place.is_a?(String) && place.match?(/\A[A-Z]\d{3}\z/)
    raise InputError, 'invalid_birthplace_code'
  end

  partial = format('%<surname>s%<name>s%<year>02d%<month>s%<day>02d%<place>s',
                   surname: encode_part(surname, name: false), name: encode_part(name, name: true),
                   year: year % 100, month: MONTH_CODES[month - 1],
                   day: sex == 'F' ? day + 40 : day, place: place)
  partial + check_character(partial)
end

begin
  input = begin
    JSON.parse($stdin.read.force_encoding(Encoding::UTF_8))
  rescue JSON::ParserError, EncodingError
    raise InputError, 'invalid_json'
  end
  puts JSON.generate(fiscal_code: fiscal_code(input))
rescue InputError => e
  puts JSON.generate(error: e.message)
  exit 1
end
