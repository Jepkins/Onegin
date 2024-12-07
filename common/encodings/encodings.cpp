#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "encodings.h"

static uint8_t utf8_cwidth (utf8_code_t c);
static uint8_t utf8_bwidth (utf8_rune_t c);

static bool utf8_iscyrillic (utf8_code_t code);
static utf8_code_t utf8_cyrillic_toupper (utf8_code_t code);
static utf8_code_t utf8_cyrillic_tolower (utf8_code_t code);

static const utf8_code_t FIRST_1BIT_MASK  = 0x80;
static const utf8_code_t FIRST_2BITS_MASK = 0xC0;
static const utf8_code_t FIRST_3BITS_MASK = 0xE0;
static const utf8_code_t FIRST_4BITS_MASK = 0xF0;
static const utf8_code_t FIRST_5BITS_MASK = 0xF8;

static const utf8_code_t LAST_6BITS_MASK = 0x3F;
static const utf8_code_t LAST_BYTE_MASK  = 0xFF;

utf8_code_t utf8_decode (utf8_rune_t rune)
{
    uint8_t width = utf8_bwidth(rune);
    utf8_code_t code = 0;

    code += (rune & (LAST_BYTE_MASK >> width)) << 6*(width - 1);

    for (uint8_t i = 1; i < width; i++)
    {
        rune = rune >> 8;
        code += (rune & LAST_6BITS_MASK) << 6*(width - i - 1);
    }

    return code;
}

utf8_rune_t utf8_encode (utf8_code_t code)
{
    uint8_t width = utf8_cwidth(code);
    char32_t rune = 0;

    for (uint8_t i = 1; i < width; i++)
    {
        rune += (FIRST_1BIT_MASK + (code & LAST_6BITS_MASK)) << 8 * (width - i);
        code = code >> 6;
    }

    rune += (((width != 1)? ~(LAST_BYTE_MASK >> width) : 0) + code);

    return rune;
}

static uint8_t utf8_cwidth (utf8_code_t c)
{
    if (c >= 0x10000)
        return 4;

    if (c >= 0x800)
        return 3;

    if (c >= 0x80)
        return 2;

    return 1;
}

static uint8_t utf8_bwidth (utf8_rune_t c)
{
    c = c & LAST_BYTE_MASK;

    if ((c & FIRST_1BIT_MASK) == 0)
        return 1;

    if (((c ^ FIRST_2BITS_MASK) & FIRST_3BITS_MASK) == 0)
        return 2;

    if (((c ^ FIRST_3BITS_MASK) & FIRST_4BITS_MASK) == 0)
        return 3;

    if (((c ^ FIRST_4BITS_MASK) & FIRST_5BITS_MASK) == 0)
        return 4;

    return 0;
}

bool utf8_validate_rune(char32_t rune, uint8_t width) // MIND: = 0
{
    if (width == 0)
    {
        width = utf8_bwidth(rune);
    }

    if (width == 0)
        return false;

    for (uint8_t i = 1; i < width; i++)
    {
        rune = rune >> 8;
        if (((rune & FIRST_2BITS_MASK) ^ FIRST_1BIT_MASK) != 0)
            return false;
    }

    return true;
}

utf8_rune_t utf8_getrune (FILE* istream)
{
    int b = getc(istream);
    if (b == EOF)
    {
        return UTF8_EOF;
    }

    utf8_rune_t rune = (utf8_rune_t) b;

    uint8_t width = utf8_bwidth(rune);

    for (int i = 1; i < width && !feof(istream); i++)
    {
        b = getc(istream);
        rune += ((utf8_rune_t) b) << (i * 8);
    }

    if (!utf8_validate_rune(rune))
    {
        return UTF8_CORRUPT;
    }

    return rune;
}

size_t utf8_getline (utf8_rune_t* buf, FILE* istream, size_t maxline) // MIND: = -1 ?
{
    size_t i = 0;

    for (; i < maxline - 1; i++)
    {
        *buf = utf8_getrune(istream);

        if (*buf == UTF8_CORRUPT)
        {
            fprintf(stderr, "%s: Corrupted text, reading not finished\n", __FUNCTION__);
            break;
        }

        if (*buf == '\n' || *buf == UTF8_EOF)
            break;

        buf++;
    }
    *buf = '\0';

    return i;
}

void utf8_putrune (utf8_rune_t rune, FILE* ostream)
{
    uint8_t width = utf8_bwidth(rune);

    fwrite(&rune, sizeof(char), width, ostream);
}

size_t utf8_putline (utf8_rune_t* rune, FILE* ostream)
{
    utf8_rune_t* beg = rune;
    while (*rune != '\0')
    {
        utf8_putrune(*rune, ostream);
        rune++;
    }
    utf8_putrune('\n', ostream);

    return (size_t)(rune - beg);
}

Alphabets utf8_isalphabetic(utf8_rune_t rune)
{
    return utf8_isalphabetic(utf8_decode(rune));
}

Alphabets utf8_isalphabetic(utf8_code_t code)
{
    if (code < 256 && isalpha((int)code))
        return ENGLISH;

    if (utf8_iscyrillic(code))
        return RUSSIAN;

    return NOT_AN_ALPHA;
}

static bool utf8_iscyrillic (utf8_code_t code)
{
    return (code >= 0x400 && code <= 0x4ff);
}

utf8_code_t utf8_toupper(utf8_code_t code)
{
    Alphabets alfb = utf8_isalphabetic(code);
    switch (alfb)
    {
        case NOT_AN_ALPHA: return code;
        case ENGLISH:      return (utf8_code_t)toupper((int)code);
        case RUSSIAN:      return utf8_cyrillic_toupper(code);
        default:           assert(0 && "Check utf8_isalphabetic return");
    }
}
utf8_code_t utf8_tolower(utf8_code_t code)
{
    Alphabets alfb = utf8_isalphabetic(code);
    switch (alfb)
    {
        case NOT_AN_ALPHA: return code;
        case ENGLISH:      return (utf8_code_t)tolower((int)code);
        case RUSSIAN:      return utf8_cyrillic_tolower(code);
        default:           assert(0 && "Check utf8_isalphabetic return");
    }
}

static utf8_code_t utf8_cyrillic_toupper (utf8_code_t code)
{
    if (code >= 0x430 && code <= 0x44f)
        return code - 0x20;
    else
        return code;
}

static utf8_code_t utf8_cyrillic_tolower (utf8_code_t code)
{
    if (code >= 0x410 && code <= 0x42f)
        return code + 0x20;
    else
        return code;
}
