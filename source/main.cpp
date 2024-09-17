#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flagging.h"
#include "text_t.h"
#include "sorting.h"

// WORK: LAST TASK, CP1251, char -> isalpha... not work, cast: char (-128, 127) -> unsigned char (0, 255) -> int (0, 255)
// WORK: utf8, 1-4byte, wchar_t + additional funcs with the type, fread -> manually to wchar_t array

int run (RUN_CONDITIONS run_conds);

int main(int argc, char *argv[])
{
    struct RUN_CONDITIONS run_conds = {};

    if (!run_setup(argc, argv, &run_conds))
        return -1;

    return run(run_conds);
}

int run (RUN_CONDITIONS run_conds)
{
    FILE* input_f_p = fopen(run_conds.input_file, "r");

    if (input_f_p == NULL)
    {
        printf("Could not open file \"%s\"", run_conds.input_file);
        return -1;
    }

    text_t text = {};
    text_ctor(&text, input_f_p);
    fclose(input_f_p);

    FILE* output_f_p = fopen(run_conds.output_file, "w");


    for (size_t i = 0; i < run_conds.sorts_n; i++)
    {
        switch (run_conds.sorts[i])
        {
            case BEG_ASC:
            {
                text_sort(&text, comp_beg_ascend);
                print_text_lines(&text, output_f_p);
                break;
            }
            case END_ASC:
            {
                text_sort(&text, comp_end_ascend);
                print_text_lines(&text, output_f_p);
                break;
            }
            case UNTOUCHED:
            {
                print_buff(&text, output_f_p);
                break;
            }
            default:
            {
                assert(0); // invalid flagging
            }
        }
        fprintf(output_f_p, "\n\n\n\n\n");
    }

    fclose(output_f_p);
    text_dtor(&text);

    return 0;
}
