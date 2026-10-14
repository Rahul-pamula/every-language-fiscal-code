import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Italian Fiscal Code (codice fiscale) — Java implementation.
 *
 * <p>Reads one JSON object from stdin and writes the result to stdout,
 * following SPECIFICATION.md. The JDK has no JSON parser, so a minimal one
 * is included.
 */
public final class FiscalCode {

    private static final String MONTH_CODES = "ABCDEHLMPRST";
    private static final List<String> FIELDS =
            List.of("surname", "name", "birth_date", "sex", "birthplace_code");
    private static final Pattern NAME = Pattern.compile("[A-Za-z '\\-]*");
    private static final Pattern DATE = Pattern.compile("(\\d{4})-(\\d{2})-(\\d{2})");
    private static final Pattern BIRTHPLACE = Pattern.compile("[A-Z]\\d{3}");
    /** Values of characters in odd positions, indexed by 0-9 then A-Z. */
    private static final int[] ODD_VALUES = {
        1, 0, 5, 7, 9, 13, 15, 17, 19, 21,
        1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3,
        6, 8, 12, 14, 16, 10, 22, 25, 24, 23,
    };

    private FiscalCode() {}

    static final class InputError extends Exception {
        private static final long serialVersionUID = 1L;

        InputError(String code) {
            super(code, null, false, false);
        }
    }

    /** Marker for JSON values that are not strings. */
    private static final Object NOT_A_STRING = new Object();

    /** Minimal JSON parser: only top-level string members are kept. */
    static final class JsonParser {
        private final String text;
        private int pos;

        JsonParser(String text) {
            this.text = text;
        }

        Map<String, Object> parseDocument() throws InputError {
            skipWhitespace();
            if (peek() != '{') throw invalid();
            Map<String, Object> members = new HashMap<>();
            parseObject(members);
            skipWhitespace();
            if (pos != text.length()) throw invalid();
            return members;
        }

        private static InputError invalid() {
            return new InputError("invalid_json");
        }

        private char peek() {
            return pos < text.length() ? text.charAt(pos) : '\0';
        }

        private char next() throws InputError {
            if (pos >= text.length()) throw invalid();
            return text.charAt(pos++);
        }

        private void expect(char c) throws InputError {
            if (next() != c) throw invalid();
        }

        private void skipWhitespace() {
            while (pos < text.length() && " \t\n\r".indexOf(text.charAt(pos)) >= 0) pos++;
        }

        private Object parseValue() throws InputError {
            skipWhitespace();
            switch (peek()) {
                case '"': return parseString();
                case '{': parseObject(null); return NOT_A_STRING;
                case '[': parseArray(); return NOT_A_STRING;
                case 't': parseLiteral("true"); return NOT_A_STRING;
                case 'f': parseLiteral("false"); return NOT_A_STRING;
                case 'n': parseLiteral("null"); return NOT_A_STRING;
                default: parseNumber(); return NOT_A_STRING;
            }
        }

        private void parseObject(Map<String, Object> members) throws InputError {
            expect('{');
            skipWhitespace();
            if (peek() == '}') {
                pos++;
                return;
            }
            while (true) {
                skipWhitespace();
                if (peek() != '"') throw invalid();
                String key = parseString();
                skipWhitespace();
                expect(':');
                Object value = parseValue();
                if (members != null) members.put(key, value);
                skipWhitespace();
                char c = next();
                if (c == '}') return;
                if (c != ',') throw invalid();
            }
        }

        private void parseArray() throws InputError {
            expect('[');
            skipWhitespace();
            if (peek() == ']') {
                pos++;
                return;
            }
            while (true) {
                parseValue();
                skipWhitespace();
                char c = next();
                if (c == ']') return;
                if (c != ',') throw invalid();
            }
        }

        private void parseLiteral(String word) throws InputError {
            if (!text.startsWith(word, pos)) throw invalid();
            pos += word.length();
        }

        private void digits() throws InputError {
            if (peek() < '0' || peek() > '9') throw invalid();
            while (peek() >= '0' && peek() <= '9') pos++;
        }

        private void parseNumber() throws InputError {
            if (peek() == '-') pos++;
            if (peek() == '0') pos++;
            else digits();
            if (peek() == '.') {
                pos++;
                digits();
            }
            if (peek() == 'e' || peek() == 'E') {
                pos++;
                if (peek() == '+' || peek() == '-') pos++;
                digits();
            }
        }

        private String parseString() throws InputError {
            expect('"');
            StringBuilder out = new StringBuilder();
            while (true) {
                char c = next();
                if (c == '"') return out.toString();
                if (c < 0x20) throw invalid();
                if (c != '\\') {
                    out.append(c);
                    continue;
                }
                char e = next();
                switch (e) {
                    case '"': case '\\': case '/': out.append(e); break;
                    case 'b': out.append('\b'); break;
                    case 'f': out.append('\f'); break;
                    case 'n': out.append('\n'); break;
                    case 'r': out.append('\r'); break;
                    case 't': out.append('\t'); break;
                    case 'u':
                        if (pos + 4 > text.length()) throw invalid();
                        try {
                            out.append((char) Integer.parseInt(text.substring(pos, pos + 4), 16));
                        } catch (NumberFormatException ex) {
                            throw invalid();
                        }
                        pos += 4;
                        break;
                    default: throw invalid();
                }
            }
        }
    }

    private static String normalizeName(Object value, String code) throws InputError {
        if (!(value instanceof String s) || !NAME.matcher(s).matches()) throw new InputError(code);
        String letters = s.toUpperCase(java.util.Locale.ROOT).replaceAll("[^A-Z]", "");
        if (letters.isEmpty()) throw new InputError(code);
        return letters;
    }

    private static String encodePart(String text, boolean isName) {
        String consonants = text.replaceAll("[AEIOU]", "");
        String vowels = text.replaceAll("[^AEIOU]", "");
        if (isName && consonants.length() >= 4) {
            return "" + consonants.charAt(0) + consonants.charAt(2) + consonants.charAt(3);
        }
        return (consonants + vowels + "XXX").substring(0, 3);
    }

    private static int[] parseDate(Object value) throws InputError {
        InputError error = new InputError("invalid_birth_date");
        if (!(value instanceof String s)) throw error;
        Matcher m = DATE.matcher(s);
        if (!m.matches()) throw error;
        int year = Integer.parseInt(m.group(1));
        int month = Integer.parseInt(m.group(2));
        int day = Integer.parseInt(m.group(3));
        boolean leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
        int[] daysInMonth = {31, leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth[month - 1]) {
            throw error;
        }
        return new int[] {year, month, day};
    }

    private static char checkCharacter(String partial) {
        int sum = 0;
        for (int i = 0; i < partial.length(); i++) {
            char c = partial.charAt(i);
            boolean digit = c >= '0' && c <= '9';
            if (i % 2 == 0) sum += ODD_VALUES[digit ? c - '0' : c - 'A' + 10];
            else sum += digit ? c - '0' : c - 'A';
        }
        return (char) ('A' + sum % 26);
    }

    static String compute(Map<String, Object> input) throws InputError {
        for (String field : FIELDS) {
            if (!input.containsKey(field)) throw new InputError("missing_field");
        }
        String surname = normalizeName(input.get("surname"), "invalid_surname");
        String name = normalizeName(input.get("name"), "invalid_name");
        int[] date = parseDate(input.get("birth_date"));
        Object sex = input.get("sex");
        if (!"M".equals(sex) && !"F".equals(sex)) throw new InputError("invalid_sex");
        if (!(input.get("birthplace_code") instanceof String place)
                || !BIRTHPLACE.matcher(place).matches()) {
            throw new InputError("invalid_birthplace_code");
        }
        String partial = encodePart(surname, false) + encodePart(name, true)
                + String.format("%02d%c%02d", date[0] % 100, MONTH_CODES.charAt(date[1] - 1),
                        "F".equals(sex) ? date[2] + 40 : date[2])
                + place;
        return partial + checkCharacter(partial);
    }

    public static void main(String[] args) throws IOException {
        String text = new String(System.in.readAllBytes(), StandardCharsets.UTF_8);
        try {
            String code = compute(new JsonParser(text).parseDocument());
            System.out.println("{\"fiscal_code\":\"" + code + "\"}");
        } catch (InputError e) {
            System.out.println("{\"error\":\"" + e.getMessage() + "\"}");
            System.exit(1);
        }
    }
}
