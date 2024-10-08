#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "text_t.h"
#include "normalizer_flagging.h"

static void print_normal_lines (text_t* text, FILE* outstream);
static bool is_normal_line (line_t* line);

static const size_t MIN_LINE_LEN = 10;
static const size_t MAX_LINE_LEN = 50;
static const size_t TAB_N = 2;

int main(int argc, char *argv[])
{
    StartConfig run_config = {};

    normalizer_setup(argc, argv, &run_config);

    text_t text = {};
    FILE* in = fopen(run_config.input_file, "r");
    text_ctor(&text, in);
    fclose(in);


    FILE* outstream = fopen(run_config.output_file, "w");
    print_normal_lines(&text, outstream);
    fclose(outstream);

    return 0;
}

static void print_normal_lines (text_t* text, FILE* outstream)
{
    for (size_t i = 0; i < text->line_n; i++)
    {
        if (is_normal_line(&text->lines[i]))
        {
            utf8_putline(text->lines[i].ptr, outstream);
        }
    }
}

static bool is_normal_line (line_t* line)
{
    size_t t = 0;
    bool alphabetic_c_met = 0;
    if (line->len < MIN_LINE_LEN || line->len > MAX_LINE_LEN)
        return false;

    for (size_t j = 0; j < line->len; j++)
    {
        if (line->ptr[j].code == 0x9)
            t++;

        if (utf8_isalphabetic(line->ptr[j].code))
        {
            alphabetic_c_met = 1;
            break;
        }
    }
    return (t == TAB_N && alphabetic_c_met);
}
