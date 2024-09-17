#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "text_t.h"
#include "flagging.h"

void print_normal_lines (text_t* text, FILE* outstream);

int main(int argc, char *argv[])
{
    RUN_CONDITIONS run_conds = {};

    normalizer_setup(argc, argv, &run_conds);

    text_t text = {};
    FILE* in = fopen(run_conds.input_file, "r");
    text_ctor(&text, in);
    fclose(in);


    FILE* outstream = fopen(run_conds.output_file, "w");
    print_normal_lines(&text, outstream);
    fclose(outstream);

    return 0;
}

void print_normal_lines (text_t* text, FILE* outstream)
{
    size_t length = 0;
    for (size_t i = 0; i < text->line_n; i++)
    {
        size_t t = 0;
        bool alpha_met = 0;
        for (size_t j = 0; j < text->lines[i].len; j++)
        {
            if (text->lines[i].ptr[j] == '\t')
                t++;

            if (isalpha(text->lines[i].ptr[j]))
            {
                alpha_met = 1;
                break;
            }
        }

        if (((length = text->lines[i].len) > 10) && length < 50 && t == 2 && alpha_met)
        {
            fputs(text->lines[i].ptr, outstream);
            fprintf(outstream, "\n");
        }
    }
}
