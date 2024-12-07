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

const size_t MAXLINE = 1000;

static void text_ctor_revert (text_t* text, ctor_errs err_num);
static void text_ctor_alarm (ctor_errs err_num);
static void set_lines (text_t* text);

bool text_ctor (text_t* text, FILE* orig_file_p)
{
    size_t size = get_text_length(orig_file_p);
    if ((int)size < 0 || ferror(orig_file_p))
    {
        text_ctor_revert(text, N_TEXT_LENGTH);
        return 0;
    }

    text->buffer = (utf8_rune_t*)calloc(size + 1, sizeof(*text->buffer));
    if (text->buffer == NULL)
    {
        text_ctor_revert(text, N_BUFF_CALLOC);
        return 0;
    }

    size_t line_n = 0, cur_p;
    while (!feof(orig_file_p))
    {
        size_t incr = utf8_getline(text->buffer + cur_p, orig_file_p, size - cur_p);
        if (incr > 0)
        {
            cur_p += incr + 1;
            line_n++;
        }
    }
    if (ferror(orig_file_p))
    {
        text_ctor_revert(text, N_FREAD);
        return 0;
    }
    text->length = cur_p;
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
        if (text->buffer[ind] == '\0')
        {
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
    long int old_p = ftell(orig_file_p);

    fseek(orig_file_p, 0, SEEK_END);
    long int len = ftell(orig_file_p);

    fseek(orig_file_p, old_p, SEEK_SET);

    return (size_t) len;
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
        if (text->buffer[i] == '\0')
            utf8_putrune('\n', out_f);

        else
            utf8_putrune(text->buffer[i], out_f);
    }
}
