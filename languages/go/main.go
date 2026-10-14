// Italian Fiscal Code (codice fiscale) — Go implementation.
//
// Reads one JSON object from stdin and writes the result to stdout,
// following SPECIFICATION.md.
package main

import (
	"encoding/json"
	"fmt"
	"io"
	"os"
	"regexp"
	"strings"
)

const monthCodes = "ABCDEHLMPRST"

var (
	nameRe       = regexp.MustCompile(`^[A-Za-z '\-]*$`)
	dateRe       = regexp.MustCompile(`^[0-9]{4}-[0-9]{2}-[0-9]{2}$`)
	birthplaceRe = regexp.MustCompile(`^[A-Z][0-9]{3}$`)
	fields       = []string{"surname", "name", "birth_date", "sex", "birthplace_code"}
	// Values of characters in odd positions, indexed by 0-9 then A-Z.
	oddValues = [36]int{
		1, 0, 5, 7, 9, 13, 15, 17, 19, 21,
		1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3,
		6, 8, 12, 14, 16, 10, 22, 25, 24, 23,
	}
)

type inputError string

func (e inputError) Error() string { return string(e) }

func normalizeName(value any, code inputError) (string, error) {
	s, ok := value.(string)
	if !ok || !nameRe.MatchString(s) {
		return "", code
	}
	letters := strings.Map(func(r rune) rune {
		if r >= 'A' && r <= 'Z' {
			return r
		}
		return -1
	}, strings.ToUpper(s))
	if letters == "" {
		return "", code
	}
	return letters, nil
}

func encodePart(text string, isName bool) string {
	var consonants, vowels []byte
	for i := 0; i < len(text); i++ {
		if strings.IndexByte("AEIOU", text[i]) >= 0 {
			vowels = append(vowels, text[i])
		} else {
			consonants = append(consonants, text[i])
		}
	}
	if isName && len(consonants) >= 4 {
		return string([]byte{consonants[0], consonants[2], consonants[3]})
	}
	return (string(consonants) + string(vowels) + "XXX")[:3]
}

func parseDate(value any) (year, month, day int, err error) {
	err = inputError("invalid_birth_date")
	s, ok := value.(string)
	if !ok || !dateRe.MatchString(s) {
		return
	}
	fmt.Sscanf(s, "%4d-%2d-%2d", &year, &month, &day)
	leap := (year%4 == 0 && year%100 != 0) || year%400 == 0
	daysInMonth := [12]int{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	if leap {
		daysInMonth[1] = 29
	}
	if year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth[month-1] {
		return
	}
	return year, month, day, nil
}

func checkCharacter(partial string) byte {
	sum := 0
	for i := 0; i < len(partial); i++ {
		c := partial[i]
		isDigit := c >= '0' && c <= '9'
		switch {
		case i%2 == 0 && isDigit:
			sum += oddValues[c-'0']
		case i%2 == 0:
			sum += oddValues[c-'A'+10]
		case isDigit:
			sum += int(c - '0')
		default:
			sum += int(c - 'A')
		}
	}
	return byte('A' + sum%26)
}

func fiscalCode(input map[string]any) (string, error) {
	for _, field := range fields {
		if _, ok := input[field]; !ok {
			return "", inputError("missing_field")
		}
	}
	surname, err := normalizeName(input["surname"], "invalid_surname")
	if err != nil {
		return "", err
	}
	name, err := normalizeName(input["name"], "invalid_name")
	if err != nil {
		return "", err
	}
	year, month, day, err := parseDate(input["birth_date"])
	if err != nil {
		return "", err
	}
	sex, _ := input["sex"].(string)
	if sex != "M" && sex != "F" {
		return "", inputError("invalid_sex")
	}
	place, ok := input["birthplace_code"].(string)
	if !ok || !birthplaceRe.MatchString(place) {
		return "", inputError("invalid_birthplace_code")
	}
	if sex == "F" {
		day += 40
	}
	partial := fmt.Sprintf("%s%s%02d%c%02d%s", encodePart(surname, false), encodePart(name, true),
		year%100, monthCodes[month-1], day, place)
	return partial + string(checkCharacter(partial)), nil
}

func run() (string, error) {
	data, err := io.ReadAll(os.Stdin)
	if err != nil {
		return "", inputError("invalid_json")
	}
	var input map[string]any
	// "null" decodes into a nil map without error, so check for it explicitly.
	if err := json.Unmarshal(data, &input); err != nil || input == nil {
		return "", inputError("invalid_json")
	}
	return fiscalCode(input)
}

func main() {
	code, err := run()
	if err != nil {
		out, _ := json.Marshal(map[string]string{"error": err.Error()})
		fmt.Println(string(out))
		os.Exit(1)
	}
	out, _ := json.Marshal(map[string]string{"fiscal_code": code})
	fmt.Println(string(out))
}
