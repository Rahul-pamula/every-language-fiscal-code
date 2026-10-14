// Italian Fiscal Code (codice fiscale) — C++ implementation.
//
// Reads one JSON object from stdin and writes the result to stdout,
// following SPECIFICATION.md. The C++ standard library has no JSON
// support, so a minimal parser is included.

#include <array>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

struct InputError : std::runtime_error {
    explicit InputError(const std::string &code) : std::runtime_error(code) {}
};

// A JSON value reduced to what the specification needs: either a string
// or "something else".
using Value = std::optional<std::string>;

class JsonParser {
public:
    explicit JsonParser(std::string_view text) : text_(text) {}

    // Parses the whole document, which must be a single object.
    std::map<std::string, Value> parseDocument() {
        skipWhitespace();
        if (peek() != '{') fail();
        std::map<std::string, Value> members;
        parseObject(&members);
        skipWhitespace();
        if (pos_ != text_.size()) fail();
        return members;
    }

private:
    std::string_view text_;
    std::size_t pos_ = 0;

    [[noreturn]] static void fail() { throw InputError("invalid_json"); }

    char peek() const { return pos_ < text_.size() ? text_[pos_] : '\0'; }

    void expect(char c) {
        if (peek() != c) fail();
        ++pos_;
    }

    void skipWhitespace() {
        while (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r') ++pos_;
    }

    Value parseValue() {
        skipWhitespace();
        switch (peek()) {
            case '"': return parseString();
            case '{': parseObject(nullptr); return std::nullopt;
            case '[': parseArray(); return std::nullopt;
            case 't': parseLiteral("true"); return std::nullopt;
            case 'f': parseLiteral("false"); return std::nullopt;
            case 'n': parseLiteral("null"); return std::nullopt;
            default: parseNumber(); return std::nullopt;
        }
    }

    void parseObject(std::map<std::string, Value> *members) {
        expect('{');
        skipWhitespace();
        if (peek() == '}') { ++pos_; return; }
        while (true) {
            skipWhitespace();
            if (peek() != '"') fail();
            std::string key = parseString();
            skipWhitespace();
            expect(':');
            Value value = parseValue();
            if (members) (*members)[key] = std::move(value);
            skipWhitespace();
            if (peek() == ',') { ++pos_; continue; }
            expect('}');
            return;
        }
    }

    void parseArray() {
        expect('[');
        skipWhitespace();
        if (peek() == ']') { ++pos_; return; }
        while (true) {
            parseValue();
            skipWhitespace();
            if (peek() == ',') { ++pos_; continue; }
            expect(']');
            return;
        }
    }

    void parseLiteral(std::string_view word) {
        if (text_.substr(pos_, word.size()) != word) fail();
        pos_ += word.size();
    }

    void digits() {
        if (!isDigit(peek())) fail();
        while (isDigit(peek())) ++pos_;
    }

    void parseNumber() {
        if (peek() == '-') ++pos_;
        if (peek() == '0') ++pos_; else digits();
        if (peek() == '.') { ++pos_; digits(); }
        if (peek() == 'e' || peek() == 'E') {
            ++pos_;
            if (peek() == '+' || peek() == '-') ++pos_;
            digits();
        }
    }

    // Non-ASCII characters are kept as raw bytes (>= 0x80); names reject them.
    std::string parseString() {
        expect('"');
        std::string out;
        while (true) {
            if (pos_ >= text_.size()) fail();
            auto c = static_cast<unsigned char>(text_[pos_++]);
            if (c == '"') return out;
            if (c < 0x20) fail();
            if (c != '\\') { out += static_cast<char>(c); continue; }
            if (pos_ >= text_.size()) fail();
            char e = text_[pos_++];
            switch (e) {
                case '"': case '\\': case '/': out += e; break;
                case 'b': out += '\b'; break;
                case 'f': out += '\f'; break;
                case 'n': out += '\n'; break;
                case 'r': out += '\r'; break;
                case 't': out += '\t'; break;
                case 'u': {
                    if (pos_ + 4 > text_.size()) fail();
                    int code = 0;
                    for (int k = 0; k < 4; ++k) {
                        char h = text_[pos_++];
                        int v = isDigit(h) ? h - '0'
                              : (h >= 'a' && h <= 'f') ? h - 'a' + 10
                              : (h >= 'A' && h <= 'F') ? h - 'A' + 10 : -1;
                        if (v < 0) fail();
                        code = code * 16 + v;
                    }
                    out += static_cast<char>(code >= 0x20 && code < 0x80 ? code : 0x80);
                    break;
                }
                default: fail();
            }
        }
    }

public:
    static bool isDigit(char c) { return c >= '0' && c <= '9'; }
};

constexpr std::string_view kVowels = "AEIOU";
constexpr std::string_view kMonthCodes = "ABCDEHLMPRST";
constexpr std::array<int, 36> kOddValues = {
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21,
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3,
    6, 8, 12, 14, 16, 10, 22, 25, 24, 23};

bool isVowel(char c) { return kVowels.find(c) != std::string_view::npos; }

std::string normalizeName(const Value &value, const std::string &error) {
    if (!value) throw InputError(error);
    std::string letters;
    for (char c : *value) {
        if (c >= 'A' && c <= 'Z') letters += c;
        else if (c >= 'a' && c <= 'z') letters += static_cast<char>(c - 'a' + 'A');
        else if (c != ' ' && c != '\'' && c != '-') throw InputError(error);
    }
    if (letters.empty()) throw InputError(error);
    return letters;
}

std::string encodePart(const std::string &text, bool isName) {
    std::string consonants, vowels;
    for (char c : text) (isVowel(c) ? vowels : consonants) += c;
    if (isName && consonants.size() >= 4) {
        return {consonants[0], consonants[2], consonants[3]};
    }
    return (consonants + vowels + "XXX").substr(0, 3);
}

struct Date { int year, month, day; };

Date parseDate(const Value &value) {
    const InputError error("invalid_birth_date");
    if (!value || value->size() != 10) throw error;
    const std::string &s = *value;
    for (std::size_t k = 0; k < s.size(); ++k) {
        bool dash = k == 4 || k == 7;
        if (dash ? s[k] != '-' : !JsonParser::isDigit(s[k])) throw error;
    }
    Date d{std::stoi(s.substr(0, 4)), std::stoi(s.substr(5, 2)), std::stoi(s.substr(8, 2))};
    bool leap = (d.year % 4 == 0 && d.year % 100 != 0) || d.year % 400 == 0;
    const std::array<int, 12> days = {31, leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (d.year < 1 || d.month < 1 || d.month > 12 || d.day < 1 || d.day > days[d.month - 1]) {
        throw error;
    }
    return d;
}

char checkCharacter(const std::string &partial) {
    int sum = 0;
    for (std::size_t k = 0; k < partial.size(); ++k) {
        char c = partial[k];
        bool digit = JsonParser::isDigit(c);
        if (k % 2 == 0) sum += kOddValues[digit ? c - '0' : c - 'A' + 10];
        else sum += digit ? c - '0' : c - 'A';
    }
    return static_cast<char>('A' + sum % 26);
}

std::string twoDigits(int n) { return (n < 10 ? "0" : "") + std::to_string(n); }

std::string fiscalCode(const std::map<std::string, Value> &input) {
    for (const char *field : {"surname", "name", "birth_date", "sex", "birthplace_code"}) {
        if (input.find(field) == input.end()) throw InputError("missing_field");
    }
    std::string surname = normalizeName(input.at("surname"), "invalid_surname");
    std::string name = normalizeName(input.at("name"), "invalid_name");
    Date date = parseDate(input.at("birth_date"));
    const Value &sex = input.at("sex");
    if (!sex || (*sex != "M" && *sex != "F")) throw InputError("invalid_sex");
    const Value &place = input.at("birthplace_code");
    if (!place || place->size() != 4 || !((*place)[0] >= 'A' && (*place)[0] <= 'Z') ||
        !JsonParser::isDigit((*place)[1]) || !JsonParser::isDigit((*place)[2]) ||
        !JsonParser::isDigit((*place)[3])) {
        throw InputError("invalid_birthplace_code");
    }

    std::string partial = encodePart(surname, false) + encodePart(name, true) +
                          twoDigits(date.year % 100) + kMonthCodes[date.month - 1] +
                          twoDigits(*sex == "F" ? date.day + 40 : date.day) + *place;
    return partial + checkCharacter(partial);
}

}  // namespace

int main() {
    std::string text{std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>()};
    try {
        std::string code = fiscalCode(JsonParser(text).parseDocument());
        std::cout << "{\"fiscal_code\":\"" << code << "\"}\n";
        return 0;
    } catch (const InputError &e) {
        std::cout << "{\"error\":\"" << e.what() << "\"}\n";
        return 1;
    }
}
