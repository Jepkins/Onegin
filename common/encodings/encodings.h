#ifndef ENCODINGS_H
#define ENCODINGS_H

#include <uchar.h>
#include <stdint.h>
#include <stdio.h>

typedef char32_t utf8_rune_t;
typedef uint32_t utf8_code_t;

static const utf8_rune_t UTF8_EOF     = (utf8_rune_t) -1;
static const utf8_rune_t UTF8_CORRUPT = (utf8_rune_t) -2;

typedef enum {
    NOT_AN_ALPHA = 0,
    ENGLISH = 1,
    RUSSIAN = 2
} Alphabets;

utf8_code_t utf8_decode (utf8_rune_t rune);
utf8_rune_t utf8_encode (utf8_code_t code);

bool utf8_validate_rune(char32_t rune, uint8_t width = 0);

utf8_rune_t utf8_getrune (FILE* istream);
size_t utf8_getline (utf8_rune_t* buf, FILE* istream, size_t maxline); // MIND: = -1 ?

void utf8_putrune (utf8_rune_t rune, FILE* ostream);
size_t utf8_putline (utf8_rune_t* runes, FILE* ostream);

Alphabets utf8_isalphabetic(utf8_rune_t rune);
Alphabets utf8_isalphabetic(utf8_code_t code);

utf8_code_t utf8_toupper(utf8_code_t code);
utf8_code_t utf8_tolower(utf8_code_t code);

#endif //ENCODINGS_H
