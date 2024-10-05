#include "normalizer_flagging.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

bool normalizer_setup(int argc, char** argv, StartConfig* run_conds)
{
    const char* optstring = " -i: -o: --input_f: --output_f: ";
    // !!! always starts and ends with ' ', short options must be before long ones, do not use ':' in option names

    getopt_out opt_out = {.optind = 1};

    GetoptResult opt_flag = ARGV_END;

    while ((opt_flag = getopt_custom(argc, argv, optstring, &opt_out)) != ARGV_END)
    {
        if (!check_opt_flag(opt_flag, opt_out))
            run_conds->flagging_error = 1;
        else
            normalizer_opt_proccessor(opt_out, run_conds);
    }

    if (!run_conds->is_input_file_selected)
    {
        run_conds->input_file = (const char*)DEFAULT_INPUT_FILE;
    }

    if (!run_conds->is_output_file_selected)
    {
        run_conds->output_file = (const char*)DEFAULT_OUTPUT_FILE;
    }

    if (run_conds->flagging_error)
        return 0;

    return 1;
}

bool normalizer_opt_proccessor (getopt_out opt_out, StartConfig *run_conds)
{
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

    if (!strcmp(opt_out.opt, "-e") || !strcmp(opt_out.opt, "--encoding"))
    {
        if (run_conds->encoding != DEFAULT)
        {
            printf("Can not select multiple encodings! (Do not use multiple -e, --encoding)\n");
            return 0;
        }

        if (!strcmp(opt_out.optarg, "utf8"))
        {
            run_conds->encoding = UTF8;
            return 1;
        }

        if (!strcmp(opt_out.optarg, "cp1251"))
        {
            run_conds->encoding = CP1251;
            return 1;
        }

        printf("Unknown encoding name: %s\n", opt_out.optarg);
        return 0;
    }

    printf("Option %s found in optstring but not in opt_proccessor()", opt_out.opt);

    return 0;
}
