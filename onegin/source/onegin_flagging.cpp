#include "onegin_flagging.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

bool onegin_setup(int argc, char** argv, StartConfig* run_conds)
{
    const char* optstring = " -i: -o: -s: --input_f: --output_f: --sorting: ";
    // !!! always starts and ends with ' ', short options must be before long ones, do not use ':' in option names

    getopt_out opt_out = {.optind = 1};

    GetoptResult opt_flag = ARGV_END;

    while ((opt_flag = getopt_custom(argc, argv, optstring, &opt_out)) != ARGV_END)
    {
        if (!check_opt_flag(opt_flag, opt_out))
            run_conds->flagging_error = 1;
        else
            onegin_opt_proccessor(opt_out, run_conds);
    }

    if (!run_conds->is_input_file_selected)
    {
        run_conds->input_file = DEFAULT_INPUT_FILE;
    }
    if (!run_conds->is_output_file_selected)
    {
        run_conds->output_file = DEFAULT_OUTPUT_FILE;
    }
    if (run_conds->sorts_n == 0)
    {
        run_conds->sorts_n = 1;
        run_conds->sorts[0] = UNSORTED;
    }

    if (run_conds->flagging_error)
        return 0;

    return 1;
}

// FUCK: copy
bool onegin_opt_proccessor (getopt_out opt_out, StartConfig *run_conds)
{
    // FUCK: think about it
    if (!strcmp(opt_out.opt, "-s") || !strcmp(opt_out.opt, "--sorting"))
    {
        if (run_conds->sorts_n == MAX_N_SORTS)
        {
            printf("Too many sorts selected! (Use 0~10)\n");
            return 0;
        }
        run_conds->sorts_n++;

        if (!strcmp(opt_out.optarg, "beg_ascend"))
        {
            run_conds->sorts[run_conds->sorts_n - 1] = BEG_ASC;
            return 1;
        }

        if (!strcmp(opt_out.optarg, "end_ascend"))
        {
            run_conds->sorts[run_conds->sorts_n - 1] = END_ASC;
            return 1;
        }

        if (!strcmp(opt_out.optarg, "unsorted"))
        {
            run_conds->sorts[run_conds->sorts_n - 1] = UNSORTED;
            return 1;
        }

        printf("Unknown sorting name: %s\n", opt_out.optarg);
        return 0;
    }

    if (!strcmp(opt_out.opt, "-i") || !strcmp(opt_out.opt, "--input_f"))
    {
        if (run_conds->is_input_file_selected)
        {
            printf("Can not select multiple files for input! (Do not use multiple -i, --input_f)\n");
            return 0;
        }

        if (strpbrk (opt_out.optarg, ":*?\"<>|") )
        {
            printf("\"%s\" is invalid file name\n", opt_out.optarg);
            return 0;
        }

        run_conds->is_input_file_selected = true;
        run_conds->input_file = opt_out.optarg;

        return 1;
    }

    if (!strcmp(opt_out.opt, "-o") || !strcmp(opt_out.opt, "--output_f"))
    {
        if (run_conds->is_output_file_selected)
        {
            printf("Can not select multiple files for output! (Do not use multiple -o, --output_f)\n");
            return 0;
        }

        if (strpbrk (opt_out.optarg, ":*?\"<>|") )
        {
            printf("\"%s\" is invalid file name\n", opt_out.optarg);
            return 0;
        }

        run_conds->is_output_file_selected = true;
        run_conds->output_file = opt_out.optarg;
        return 1;
    }

    printf("Option %s found in optstring but not in set_run_flags()", opt_out.opt);

    return 0;
}
