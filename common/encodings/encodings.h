#ifndef ENCODINGS_H
#define ENCODINGS_H

#include <uchar.h>
#include <stdint.h>

typedef struct {
    char32_t bits;
    uint32_t code;
    uint8_t width;
} utf8_rune_t;

typedef enum {
    NOT_AN_ALPHA = 0,
    ENGLISH = 1,
    RUSSIAN = 2
} Alphabets;

uint32_t utf8_decode (char32_t c);
char32_t utf8_encode (uint32_t code);
uint8_t utf8_bwidth (char32_t c);
uint8_t utf8_cwidth (uint32_t c);

bool check_following_bytes(char32_t bits);

void utf8_set_null (utf8_rune_t* r);
bool utf8_isnull (utf8_rune_t* r);

void utf8_set_by_code (utf8_rune_t* r, uint32_t code);

void utf8_getrune (utf8_rune_t* r, FILE* istream);
size_t utf8_getline (utf8_rune_t* buf, size_t maxline, FILE* istream);
void utf8_putrune (utf8_rune_t* r, FILE* ostream);
size_t utf8_putline (utf8_rune_t* r, FILE* ostream);

void utf8_put_lf(FILE* ostream);

Alphabets utf8_isalphabetic (uint32_t code);

bool utf8_iscyrillic (uint32_t code);
bool utf8_iscyrillic (char32_t bits);

uint32_t utf8_toupper (uint32_t code);
uint32_t utf8_tolower (uint32_t code);

uint32_t utf8_cyrillic_toupper (uint32_t code);
uint32_t utf8_cyrillic_toupper (char32_t bits);
uint32_t utf8_cyrillic_tolower (uint32_t code);
uint32_t utf8_cyrillic_tolower (char32_t bits);

void dump_rune (utf8_rune_t* r);

#endif //ENCODINGS_H
