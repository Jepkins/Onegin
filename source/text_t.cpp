#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "text_t.h"

enum ctor_errs {
    N_TEXT_LENGTH = -1,
    N_BUFF_CALLOC = -2,
    N_FREAD = -3,
    N_LINES_CALLOC = -4
};

void text_ctor_revert (text_t* text, ctor_errs err_num);
void text_ctor_alarm(ctor_errs err_num);
void set_lines (text_t* text);

void text_ctor(text_t* text, FILE* orig_file_p)
{
    size_t size = get_text_length(orig_file_p);
    if (size == (size_t)-1)
    {
        text_ctor_revert(text, N_TEXT_LENGTH);
        return;
    }
    text->buffer = (char*)calloc(size + 1, sizeof(*text->buffer));

    if (text->buffer == NULL)
    {
        text_ctor_revert(text, N_BUFF_CALLOC);
        return;
    }

    size = fread(text->buffer, sizeof(*text->buffer), size, orig_file_p);
    if (ferror(orig_file_p))
    {
        text_ctor_revert(text, N_FREAD);
        return;
    }
    text->length = size;

    text->line_n = count_lines(text);
    if (text->length == 0 || text->buffer[text->length - 1] != '\n')
    {
        text->buffer[text->length] = '\0';
        text->length++;
        text->line_n++;
    }

    text->lines = (line_t*)calloc(text->line_n + 1, sizeof(*text->lines));
    if (text->lines == NULL)
    {
        text_ctor_revert(text, N_LINES_CALLOC);
        return;
    }

    set_lines(text);
}

void set_lines (text_t* text)
{
    text->lines[0].ptr = text->buffer;

    char* l_beg = text->buffer;

    for (size_t c = 0, line = 0, len = 0; c < text->length; c++)
    {
        if (text->buffer[c] == '\n')
        {
            text->buffer[c] = '\0';

            text->lines[line].len = len - 1;
            text->lines[line].ptr = l_beg;
            line++;

            l_beg = text->buffer + c + 1;
            len = 0;
        }
        len++;
    }
}

void text_ctor_revert (text_t* text, ctor_errs err_code)
{
    for (int i = err_code; i < 0; i++)
    {
        switch (i)
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
    }

    text_ctor_alarm(err_code);
}

void text_ctor_alarm(ctor_errs err_code)
{
    printf("Error occured in text_ctor() (err_code = %d). Break.\n", err_code);
    exit(err_code);
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

size_t count_lines(text_t* text)
{
    size_t cnt = 0;

    for (size_t c = 0; c < text->length; c++)
    {
        if (text->buffer[c] == '\n')
            cnt++;
    }

    return cnt;
}

void print_text_lines (text_t* text, FILE* out_f)
{
    for (size_t i = 0; i < text->line_n; i++)
    {
        fputs(text->lines[i].ptr, out_f); // FUCK: change (why???)
        fputc('\n', out_f);
    }
}

void print_buff (text_t* text, FILE* out_f)
{
    for (size_t i = 0; i < text->length; i++)
    {
        if (text->buffer[i] == '\0')
            fputc('\n', out_f);

        else
            fputc(text->buffer[i], out_f);
    }
}
