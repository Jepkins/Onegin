#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "encodings.h"


const uint32_t LAST_6BITS_MASK = 0x3f;
const uint32_t LAST_BYTE_MASK = 0xff;

static utf8_rune_t UTF8_n = {0x0A000000, 0xA, 1};

uint32_t utf8_decode (char32_t bits)
{
    uint8_t width = utf8_bwidth(bits);
    uint32_t code = 0;

    bits = bits >> 8*(4 - width);

    for (uint8_t i = 1; i < width; i++)
    {
        code += (bits & LAST_6BITS_MASK) << 6*(i - 1);
        bits = bits >> 8;
    }

    code += (bits & (LAST_BYTE_MASK >> width)) << 6*(width - 1);

    return code;
}

char32_t utf8_encode (uint32_t code)
{
    uint8_t width = utf8_cwidth(code);
    char32_t bits = 0;

    for (uint8_t i = 1; i < width; i++)
    {
        bits += (0x80 + (code & LAST_6BITS_MASK)) << 8*(i - 1);
        code = code >> 6;
    }

    bits += (((width != 1)? ~(LAST_BYTE_MASK >> width) : 0) + code) << 8*(width - 1);

    return bits;
}

uint8_t utf8_cwidth (uint32_t c)
{
    if (c >= 200000)
        return 0;

    if (c >= 0x10000)
        return 4;
    if (c >= 0x800)
        return 3;
    if (c >= 0x80)
        return 2;

    return 1;
}

uint8_t utf8_bwidth (char32_t c)
{
    if (!(c >> (sizeof(c)*8-1)))
        return 1;

    c = c << 1;
    uint8_t w = 1;

    while (c >> (sizeof(c)*8-1))
    {
        w++;
        c = c << 1;
    }
    if (w < 5)
    {
        return w;
    }
    else
    {
        return 0;
    }

}

bool check_following_bytes(char32_t bits)
{
    uint8_t width = utf8_bwidth(bits);
    bits = bits >> 8*(4 - width);
    for (uint8_t i = 1; i < width; i++)
    {
        if ((bits & 0xC0) ^ 0x80)
            return false;
        bits = bits >> 8;
    }
    return true;
}

void utf8_set_null (utf8_rune_t* r)
{
    r->width = 1;
    r->bits = 0;
    r->code = 0;
}

bool utf8_isnull (utf8_rune_t* r)
{
    return r->width == 1 &&
           r->bits == 0 &&
           r->code == 0;
}

void utf8_set_by_code (utf8_rune_t* r, uint32_t code)
{
    r->code = code;
    r->width = utf8_cwidth(code);
    r->bits = utf8_encode(code);
}


void utf8_getrune (utf8_rune_t* r, FILE* istream)
{
    if(feof(istream))
    {
        utf8_set_by_code(r, (uint32_t)-1);
        return;
    }

    unsigned char b = (unsigned char)getc(istream);

    r->bits = ((char32_t)b) << 24;

    r->width = utf8_bwidth(r->bits);

    for (int i = 1; i < r->width && !feof(istream); i++)
    {
        b = (unsigned char)getc(istream);
        r->bits += ((char32_t)b) << (24 - i*8);
    }
    if(!check_following_bytes(r->bits))
    {
        utf8_set_by_code(r, (uint32_t)-1);
        return;
    }
    r->code = utf8_decode(r->bits);
}

void dump_rune (utf8_rune_t* r)
{
    printf("dump: bits = %x, code = %x, width = %d\n", r->bits, r->code, r->width); // MORON: nessesary?
}

size_t utf8_getline (utf8_rune_t* buf, size_t maxline, FILE* istream)
{
    size_t i = 0;
    for (; i < maxline - 1 && !feof(istream); i++)
    {
        utf8_getrune(buf, istream);
        if (buf->code == UTF8_n.code || buf->width == 0)
            break;

        buf++;
    }
    utf8_set_null(buf);
    return i;
}

void utf8_putrune (utf8_rune_t* r, FILE* ostream)
{
    for (int i = 0; i < r->width; i++)
    {
        fputc((unsigned char)(r->bits >> (3 - i)*8), ostream);
    }
}

size_t utf8_putline (utf8_rune_t* r, FILE* ostream)
{
    utf8_rune_t* beg = r;
    while (r->code != 0)
    {
        utf8_putrune(r, ostream);
        r++;
    }
    utf8_put_lf(ostream);
    return (size_t)(r - beg);
}

void utf8_put_lf(FILE* ostream)
{
    utf8_putrune(&UTF8_n, ostream);
}

Alphabets utf8_isalphabetic(uint32_t code)
{
    if (code < 256 && isalpha((int)code))
        return ENGLISH;

    if (utf8_iscyrillic(code))
        return RUSSIAN;

    return NOT_AN_ALPHA;
}

bool utf8_iscyrillic (uint32_t code)
{
    return (code >= 0x400 && code <= 0x4ff);
}
bool utf8_iscyrillic (char32_t bits)
{
    return utf8_iscyrillic(utf8_decode(bits));
}

uint32_t utf8_toupper(uint32_t code)
{
    Alphabets alfb = utf8_isalphabetic(code);
    switch (alfb)
    {
        case NOT_AN_ALPHA: return code;
        case ENGLISH:      return (uint32_t)toupper((int)code);
        case RUSSIAN:      return utf8_cyrillic_toupper(code);
        default:           assert(0 && "Check utf8_isalphabetic return");
    }
}
uint32_t utf8_tolower(uint32_t code)
{
    Alphabets alfb = utf8_isalphabetic(code);
    switch (alfb)
    {
        case NOT_AN_ALPHA: return code;
        case ENGLISH:      return (uint32_t)tolower((int)code);
        case RUSSIAN:      return utf8_cyrillic_tolower(code);
        default:           assert(0 && "Check utf8_isalphabetic return");
    }
}

uint32_t utf8_cyrillic_toupper (uint32_t code)
{
    if (code >= 0x430 && code <= 0x44f)
        return code - 0x20;
    else
        return code;
}
uint32_t utf8_cyrillic_toupper (char32_t bits)
{
    return utf8_decode(utf8_cyrillic_toupper(utf8_decode(bits)));
}

uint32_t utf8_cyrillic_tolower (uint32_t code)
{
    if (code >= 0x410 && code <= 0x42f)
        return code + 0x20;
    else
        return code;
}
uint32_t utf8_cyrillic_tolower (char32_t bits)
{
    return utf8_decode(utf8_cyrillic_toupper(utf8_decode(bits)));
}
