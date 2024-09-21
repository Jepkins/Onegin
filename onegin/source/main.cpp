#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flagging.h"
#include "text_t.h"
#include "sorting.h"

// WORK: LAST TASK, CP1251, char -> isalpha... not work, cast: char (-128, 127) -> unsigned char (0, 255) -> int (0, 255)
// WORK: utf8, 1-4byte, char32_t + additional funcs with the type, fread -> manually to char32_t array

int read_sort_write (StartConfig* run_config);
int sort_write(text_t* text, StartConfig* run_config, FILE* output_file);

int main(int argc, char *argv[])
{
    time_t start = clock();

    StartConfig run_config = {};

    if (!run_setup(argc, argv, &run_config))
        return -1;

    int ret_value = read_sort_write(&run_config);

    time_t end = clock();
    printf("TIME = %lld\n", end-start);

    return ret_value;
}

int read_sort_write (StartConfig* run_config)
{
    FILE* input_file = fopen(run_config->input_file, "r");
    if (input_file == NULL)
    {
        printf("Could not open file \"%s\"", run_config->input_file);
        return -1;
    }

    text_t text = {};
    if (!text_ctor(&text, input_file))
        return -1;

    fclose(input_file);

    FILE* output_file = fopen(run_config->output_file, "w");
    if (output_file == NULL)
    {
        printf("Could not open file \"%s\"", run_config->output_file);
        return -1;
    }

    sort_write(&text, run_config, output_file);

    fclose(output_file);
    text_dtor(&text);

    return 0;
}

int sort_write(text_t* text, StartConfig* run_config, FILE* output_file)
{
    for (size_t i = 0; i < run_config->sorts_n; i++)
    {
        if (text->state == run_config->sorts[i])
        {
            print_text_lines(text, output_file);
            continue;
        }

        switch (run_config->sorts[i])
        {
            case BEG_ASC:
            {
                fprintf(output_file, "\n\n\nSorting: beg_ascend\n");
                text_sort(text, comp_beg_ascend);
                print_text_lines(text, output_file);
                break;
            }
            case END_ASC:
            {
                fprintf(output_file, "\n\n\nSorting: end_ascend\n");
                text_sort(text, comp_end_ascend);
                print_text_lines(text, output_file);
                break;
            }
            case UNSORTED:
            {
                fprintf(output_file, "\n\n\nSorting: unsorted\n");
                print_buff(text, output_file);
                break;
            }
            default:
            {
                assert(0 && "invalid flagging");
            }
        }
        text->state = run_config->sorts[i];
    }

    return 0;
}
