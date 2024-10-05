#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "text_t.h"

enum ctor_errs {
    N_TEXT_LENGTH = -1,
    N_BUFF_CALLOC = -2,
    N_FREAD = -3,
    N_LINES_CALLOC = -4
};

static utf8_rune_t UTF8_n = {0x0A000000, 0xA, 1};

const size_t MAXLINE = 1000;

static void text_ctor_revert (text_t* text, ctor_errs err_num);
static void text_ctor_alarm (ctor_errs err_num);
static void set_lines (text_t* text);

bool text_ctor (text_t* text, FILE* orig_file_p)
{
    size_t temp_size = get_text_length(orig_file_p);
    if (temp_size == (size_t)-1)
    {
        text_ctor_revert(text, N_TEXT_LENGTH);
        return 0;
    }

    text->buffer = (utf8_rune_t*)calloc(temp_size + 1, sizeof(*text->buffer));
    if (text->buffer == NULL)
    {
        text_ctor_revert(text, N_BUFF_CALLOC);
        return 0;
    }

    size_t size = 0, line_n = 0;
    while (!feof(orig_file_p))
    {
        size_t incr = utf8_getline(text->buffer + size, MAXLINE, orig_file_p);
        if (incr > 0)
        {
            size += incr + 1;
            line_n++;
        }
    }
    if (ferror(orig_file_p))
    {
        text_ctor_revert(text, N_FREAD);
        return 0;
    }
    text->length = size;
    text->line_n = line_n;

    text->lines = (line_t*)calloc(text->line_n + 1, sizeof(*text->lines));
    if (text->lines == NULL)
    {
        text_ctor_revert(text, N_LINES_CALLOC);
        return 0;
    }

    set_lines(text);

    return 1;
}

static void set_lines (text_t* text)
{
    text->lines[0].ptr = text->buffer;

    utf8_rune_t* l_beg = text->buffer;

    for (size_t ind = 0, line = 0, len = 0; ind < text->length; ind++)
    {
        if (utf8_isnull(&text->buffer[ind]))
        {
            text->buffer[ind].bits = 0;
            text->buffer[ind].code = 0;

            text->lines[line].len = len - 1;
            text->lines[line].ptr = l_beg;
            line++;

            l_beg = text->buffer + ind + 1;
            len = 0;
        }
        len++;
    }
}

static void text_ctor_revert (text_t* text, ctor_errs err_code)
{
    switch (err_code)
    {
        case N_LINES_CALLOC:
        {
            text->length = 0;
            text->line_n = 0;
            [[fallthrough]];
        }
        case N_FREAD:
        {
            free(text->buffer);
            text->buffer = NULL;
            [[fallthrough]];
        }
        default:
            break;
    }

    text_ctor_alarm(err_code);
}

static void text_ctor_alarm(ctor_errs err_code)
{
    printf("Error occured in text_ctor() (err_code = %d). Break.\n", err_code);
}

void text_dtor(text_t* text)
{
    free(text->buffer);
    free(text->lines);

    text->buffer = NULL;
    text->lines = NULL;
    text->length = 0;
    text->line_n = 0;
}

size_t get_text_length(FILE* orig_file_p)
{
    struct stat statbuf = {};
    if (fstat(fileno(orig_file_p), &statbuf) != 0)
    {
        return (size_t)-1;
    }

    return (size_t)statbuf.st_size;
}


void print_text_lines (text_t* text, FILE* out_f)
{
    for (size_t i = 0; i < text->line_n; i++)
    {
        utf8_putline(text->lines[i].ptr, out_f);
    }
    fprintf(out_f, "\n\n\n");
}

void print_buff (text_t* text, FILE* out_f)
{
    for (size_t i = 0; i < text->length; i++)
    {
        if (text->buffer[i].code == 0)
            utf8_putrune(&UTF8_n, out_f);

        else
            utf8_putrune(&text->buffer[i], out_f);
    }
}

void text_dump (text_t* text, FILE* ostream)
{
    assert(text && ostream);
    fprintf(ostream, "TEXT_DUMP, time = %ld\n", clock());
    fprintf(ostream, "text->length = %ld\n", text->length);
    fprintf(ostream, "text->line_n = %ld\n", text->line_n);
    fprintf(ostream, "text->state code = %d\n", text->state);

    fprintf(ostream, "text->buffer:\n{\n");
    for (size_t i = 0; i < text->length; i++)
    {
        fprintf(ostream, "[%ld] = ", i);
        utf8_putrune(text->buffer + i, ostream);
        fprintf(ostream, "(code = %d)\n", text->buffer[i].code);
        fprintf(ostream, "(width = %d)\n", text->buffer[i].width);
    }
    fprintf(ostream, "}\n\n");

    fprintf(ostream, "text->lines:\n{\n");
    for (size_t i = 0; i < text->line_n; i++)
    {
        fprintf(ostream, "[%ld](%p) = ", i, (void*)text->lines[i].ptr);
        utf8_putline(text->lines[i].ptr, ostream);
    }
    fprintf(ostream, "}\n\n");
}
