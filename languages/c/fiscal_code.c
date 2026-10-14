/*
 * Italian Fiscal Code (codice fiscale) — C implementation.
 *
 * Reads one JSON object from stdin and writes the result to stdout,
 * following SPECIFICATION.md. C has no standard JSON library, so this
 * file contains a small JSON parser.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_UPPER(c) ((c) >= 'A' && (c) <= 'Z')

enum { T_ERROR = -1, T_STRING, T_OTHER };
enum { F_SURNAME, F_NAME, F_BIRTH_DATE, F_SEX, F_BIRTHPLACE, F_COUNT };

static const char *FIELD_NAMES[F_COUNT] = {
    "surname", "name", "birth_date", "sex", "birthplace_code"
};
static const char *MONTH_CODES = "ABCDEHLMPRST";
/* Values of characters in odd positions, indexed by 0-9 then A-Z. */
static const int ODD_VALUES[36] = {
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21,
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3,
    6, 8, 12, 14, 16, 10, 22, 25, 24, 23
};

typedef struct {
    int present;
    int is_string;
    char *value;
} Field;

typedef struct {
    const char *s;
    size_t i, n;
} Parser;

/* ------------------------------------------------------------------ */
/* JSON parsing                                                       */
/* ------------------------------------------------------------------ */

static char peek(const Parser *p) { return p->i < p->n ? p->s[p->i] : '\0'; }

static void skip_ws(Parser *p)
{
    char c;
    while ((c = peek(p)) == ' ' || c == '\t' || c == '\n' || c == '\r')
        p->i++;
}

static int hex_value(char c)
{
    if (IS_DIGIT(c)) return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* Parses a string; non-ASCII characters are kept as bytes >= 0x80. */
static int parse_string(Parser *p, char **out)
{
    char *buf = malloc(p->n - p->i + 1);
    size_t len = 0;

    p->i++; /* opening quote */
    for (;;) {
        unsigned char c;
        if (p->i >= p->n) goto fail;
        c = (unsigned char)p->s[p->i++];
        if (c == '"') break;
        if (c < 0x20) goto fail;
        if (c != '\\') {
            buf[len++] = (char)c;
            continue;
        }
        if (p->i >= p->n) goto fail;
        c = (unsigned char)p->s[p->i++];
        switch (c) {
        case '"': case '\\': case '/': buf[len++] = (char)c; break;
        case 'b': buf[len++] = '\b'; break;
        case 'f': buf[len++] = '\f'; break;
        case 'n': buf[len++] = '\n'; break;
        case 'r': buf[len++] = '\r'; break;
        case 't': buf[len++] = '\t'; break;
        case 'u': {
            int k, code = 0;
            for (k = 0; k < 4; k++) {
                int h = p->i < p->n ? hex_value(p->s[p->i]) : -1;
                if (h < 0) goto fail;
                code = code * 16 + h;
                p->i++;
            }
            /* Anything outside printable ASCII is invalid in names anyway. */
            buf[len++] = (char)(code >= 0x20 && code < 0x80 ? code : 0x80);
            break;
        }
        default: goto fail;
        }
    }
    buf[len] = '\0';
    if (out) *out = buf; else free(buf);
    return T_STRING;
fail:
    free(buf);
    return T_ERROR;
}

static int parse_number(Parser *p)
{
    if (peek(p) == '-') p->i++;
    if (!IS_DIGIT(peek(p))) return T_ERROR;
    if (peek(p) == '0') p->i++;
    else while (IS_DIGIT(peek(p))) p->i++;
    if (peek(p) == '.') {
        p->i++;
        if (!IS_DIGIT(peek(p))) return T_ERROR;
        while (IS_DIGIT(peek(p))) p->i++;
    }
    if (peek(p) == 'e' || peek(p) == 'E') {
        p->i++;
        if (peek(p) == '+' || peek(p) == '-') p->i++;
        if (!IS_DIGIT(peek(p))) return T_ERROR;
        while (IS_DIGIT(peek(p))) p->i++;
    }
    return T_OTHER;
}

static int parse_literal(Parser *p, const char *word)
{
    size_t len = strlen(word);
    if (p->n - p->i < len || strncmp(p->s + p->i, word, len) != 0) return T_ERROR;
    p->i += len;
    return T_OTHER;
}

static int parse_value(Parser *p, char **str);

static void record_field(Field *fields, const char *key, int type, char *value)
{
    int k;
    for (k = 0; k < F_COUNT; k++) {
        if (strcmp(key, FIELD_NAMES[k]) == 0) {
            free(fields[k].value);
            fields[k].present = 1;
            fields[k].is_string = type == T_STRING;
            fields[k].value = value;
            return;
        }
    }
    free(value);
}

/* Parses an object; when fields is not NULL, records the known members. */
static int parse_object(Parser *p, Field *fields)
{
    p->i++; /* '{' */
    skip_ws(p);
    if (peek(p) == '}') { p->i++; return T_OTHER; }
    for (;;) {
        char *key = NULL, *value = NULL;
        int type;
        skip_ws(p);
        if (peek(p) != '"' || parse_string(p, &key) == T_ERROR) return T_ERROR;
        skip_ws(p);
        if (peek(p) != ':') { free(key); return T_ERROR; }
        p->i++;
        type = parse_value(p, &value);
        if (type == T_ERROR) { free(key); return T_ERROR; }
        if (fields) record_field(fields, key, type, value);
        else free(value);
        free(key);
        skip_ws(p);
        if (peek(p) == ',') { p->i++; continue; }
        if (peek(p) == '}') { p->i++; return T_OTHER; }
        return T_ERROR;
    }
}

static int parse_array(Parser *p)
{
    p->i++; /* '[' */
    skip_ws(p);
    if (peek(p) == ']') { p->i++; return T_OTHER; }
    for (;;) {
        if (parse_value(p, NULL) == T_ERROR) return T_ERROR;
        skip_ws(p);
        if (peek(p) == ',') { p->i++; continue; }
        if (peek(p) == ']') { p->i++; return T_OTHER; }
        return T_ERROR;
    }
}

static int parse_value(Parser *p, char **str)
{
    skip_ws(p);
    switch (peek(p)) {
    case '"': return parse_string(p, str);
    case '{': return parse_object(p, NULL);
    case '[': return parse_array(p);
    case 't': return parse_literal(p, "true");
    case 'f': return parse_literal(p, "false");
    case 'n': return parse_literal(p, "null");
    default:  return parse_number(p);
    }
}

/* ------------------------------------------------------------------ */
/* Fiscal code                                                        */
/* ------------------------------------------------------------------ */

/* Writes the upper-case letters of value into out; 0 if invalid. */
static int normalize_name(const char *value, char *out)
{
    size_t len = 0;
    for (; *value; value++) {
        char c = *value;
        if (IS_UPPER(c)) out[len++] = c;
        else if (c >= 'a' && c <= 'z') out[len++] = (char)(c - 'a' + 'A');
        else if (c != ' ' && c != '\'' && c != '-') return 0;
    }
    out[len] = '\0';
    return len > 0;
}

static int is_vowel(char c) { return strchr("AEIOU", c) != NULL; }

static void encode_part(const char *text, int is_name, char *out)
{
    size_t n = strlen(text), nc = 0, nv = 0, k;
    char *consonants = malloc(n + 1), *letters = malloc(n + 4);

    for (k = 0; k < n; k++)
        if (!is_vowel(text[k])) consonants[nc++] = text[k];
    if (is_name && nc >= 4) {
        out[0] = consonants[0];
        out[1] = consonants[2];
        out[2] = consonants[3];
    } else {
        memcpy(letters, consonants, nc);
        for (k = 0; k < n; k++)
            if (is_vowel(text[k])) letters[nc + nv++] = text[k];
        memcpy(letters + nc + nv, "XXX", 3);
        memcpy(out, letters, 3);
    }
    out[3] = '\0';
    free(consonants);
    free(letters);
}

static int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

static int parse_date(const char *s, int *year, int *month, int *day)
{
    static const int DAYS[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int k, max_day;

    if (strlen(s) != 10 || s[4] != '-' || s[7] != '-') return 0;
    for (k = 0; k < 10; k++)
        if (k != 4 && k != 7 && !IS_DIGIT(s[k])) return 0;
    *year = (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + (s[3] - '0');
    *month = (s[5] - '0') * 10 + (s[6] - '0');
    *day = (s[8] - '0') * 10 + (s[9] - '0');
    if (*year < 1 || *month < 1 || *month > 12) return 0;
    max_day = DAYS[*month - 1] + (*month == 2 && is_leap_year(*year));
    return *day >= 1 && *day <= max_day;
}

static int valid_birthplace(const char *s)
{
    return strlen(s) == 4 && IS_UPPER(s[0]) && IS_DIGIT(s[1]) && IS_DIGIT(s[2]) && IS_DIGIT(s[3]);
}

static char check_character(const char *partial)
{
    int k, sum = 0;
    for (k = 0; partial[k]; k++) {
        char c = partial[k];
        int index = IS_DIGIT(c) ? c - '0' : c - 'A' + 10;
        if (k % 2 == 0) sum += ODD_VALUES[index];
        else sum += IS_DIGIT(c) ? c - '0' : c - 'A';
    }
    return (char)('A' + sum % 26);
}

static const char *compute(Field *f, char *result)
{
    static const char *ERRORS[F_COUNT] = {
        "invalid_surname", "invalid_name", "invalid_birth_date",
        "invalid_sex", "invalid_birthplace_code"
    };
    char *surname, *name, code[4];
    int k, year = 0, month = 1, day = 1, failed;

    for (k = 0; k < F_COUNT; k++)
        if (!f[k].present) return "missing_field";
    for (k = 0; k < F_COUNT; k++)
        if (!f[k].is_string) return ERRORS[k];

    surname = malloc(strlen(f[F_SURNAME].value) + 1);
    name = malloc(strlen(f[F_NAME].value) + 1);
    if (!normalize_name(f[F_SURNAME].value, surname)) failed = F_SURNAME;
    else if (!normalize_name(f[F_NAME].value, name)) failed = F_NAME;
    else if (!parse_date(f[F_BIRTH_DATE].value, &year, &month, &day)) failed = F_BIRTH_DATE;
    else if (strcmp(f[F_SEX].value, "M") != 0 && strcmp(f[F_SEX].value, "F") != 0) failed = F_SEX;
    else if (!valid_birthplace(f[F_BIRTHPLACE].value)) failed = F_BIRTHPLACE;
    else failed = -1;

    if (failed < 0) {
        encode_part(surname, 0, code);
        strcpy(result, code);
        encode_part(name, 1, code);
        strcat(result, code);
        sprintf(result + 6, "%02d%c%02d%s", year % 100, MONTH_CODES[month - 1],
                f[F_SEX].value[0] == 'F' ? day + 40 : day, f[F_BIRTHPLACE].value);
        result[15] = check_character(result);
        result[16] = '\0';
    }
    free(surname);
    free(name);
    return failed < 0 ? NULL : ERRORS[failed];
}

static char *read_stdin(size_t *len)
{
    size_t cap = 4096, n;
    char *buf = malloc(cap);
    *len = 0;
    while ((n = fread(buf + *len, 1, cap - *len, stdin)) > 0) {
        *len += n;
        if (*len == cap) buf = realloc(buf, cap *= 2);
    }
    return buf;
}

int main(void)
{
    Field fields[F_COUNT] = { { 0, 0, NULL } };
    Parser p;
    char result[32];
    const char *error = NULL;
    int k;

    p.s = read_stdin(&p.n);
    p.i = 0;
    skip_ws(&p);
    if (peek(&p) != '{' || parse_object(&p, fields) == T_ERROR) {
        error = "invalid_json";
    } else {
        skip_ws(&p);
        if (p.i != p.n) error = "invalid_json";
    }
    if (!error) error = compute(fields, result);

    if (error) printf("{\"error\":\"%s\"}\n", error);
    else printf("{\"fiscal_code\":\"%s\"}\n", result);

    for (k = 0; k < F_COUNT; k++) free(fields[k].value);
    free((char *)p.s);
    return error ? 1 : 0;
}
