#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "flagging.h"

bool run_setup(int argc, char** argv, RUN_CONDITIONS* run_conds)
{
    const char* optstring = " -i: -o: -s: --input_f: --output_f: --sorting: ";
    // !!! always starts and ends with ' ', short options must be before long ones, do not use ':' in option names

    if (argc == 2 && !strcmp(argv[1], "help")) {
        print_help();
        return 0;
    }

    struct getopt_out opt_out = {.optind = 1};

    enum GETOPT_RETS opt_flag = ARGV_END;

    while ((opt_flag = getopt_custom(argc, argv, optstring, &opt_out)) != ARGV_END)
    {
        if (!set_run_flags(opt_flag, opt_out, run_conds))
            run_conds->flagging_error = 1;
    }

    if (!run_conds->input_file_selected)
    {
        run_conds->input_file = (const char*)"data/text_rus_norm.txt";
    }
    if (!run_conds->output_file_selected)
    {
        run_conds->output_file = (const char*)"data/output.txt";
    }
    if (run_conds->sorts_n == 0)
    {
        run_conds->sorts_n = 1;
        run_conds->sorts[0] = UNTOUCHED;
    }

    if (run_conds->flagging_error)
        return 0;

    return 1;
}

bool normalizer_setup(int argc, char** argv, RUN_CONDITIONS* run_conds)
{
    const char* optstring = " -i: -o: --input_f: --output_f: ";
    // !!! always starts and ends with ' ', short options must be before long ones, do not use ':' in option names

    struct getopt_out opt_out = {.optind = 1};

    enum GETOPT_RETS opt_flag = ARGV_END;

    while ((opt_flag = getopt_custom(argc, argv, optstring, &opt_out)) != ARGV_END)
    {
        if (!set_run_flags(opt_flag, opt_out, run_conds))
            run_conds->flagging_error = 1;
    }

    if (!run_conds->input_file_selected)
    {
        run_conds->input_file = (const char*)"data/text_rus.txt";
    }

    if (!run_conds->output_file_selected)
    {
        run_conds->output_file = (const char*)"data/text_rus_norm.txt";
    }

    if (run_conds->flagging_error)
        return 0;

    return 1;
}

GETOPT_RETS getopt_custom(int argc, char ** const argv, const char *optstring, getopt_out* opt_out)
{
    if (opt_out->optind > argc - 1)
        return ARGV_END;

    char* word = argv[opt_out->optind++];
    opt_out->opt = word;

    size_t word_length = strlen(word);

    if (word_length < 2 || word[0] != '-' || (word_length == 2 && word[1] == '-') || (word_length > 2 && word[1] != '-'))
        return NOT_AN_OPT;

    char* opt_pointer = strstr(optstring, word);

    if (opt_pointer == NULL)
        return OPT_UNKNOWN;

    if (opt_pointer == optstring || opt_pointer[word_length] == NULL)
        return INVALID_OPTSTR;

    if (opt_pointer[-1] != ' ' || (opt_pointer[word_length] != ':' && opt_pointer[word_length] != ' '))
        return OPT_UNKNOWN;

    if (opt_pointer[word_length] == ':')
        return getoptarg(argc, argv, opt_out);

    return OPT_FOUND;
}

GETOPT_RETS getoptarg(int argc, char ** const argv, getopt_out* opt_out)
{
    if (opt_out->optind > argc - 1)
        return EXPECTED_ARG;

    if (*(argv + opt_out->optind)[0] == '-')
        return EXPECTED_ARG;

    opt_out->optarg = argv[opt_out->optind++];

    return OPT_FOUND;
}

bool set_run_flags(GETOPT_RETS opt_flag, getopt_out opt_out, RUN_CONDITIONS *run_conds)
{
    switch (opt_flag)
    {
        case OPT_FOUND:
        {
            return opt_proccessor(opt_out, run_conds);
        }

        case NOT_AN_OPT:
        {
            printf(
                "Expected option: %s  \n"
                "                 ^   \n"
                "                 |   \n"
                "                 here\n",
                opt_out.opt
            );
            return 0;
        }

        case OPT_UNKNOWN:
        {
            printf("Unknown option: %s\n", opt_out.opt);
            return 0;
        }

        case EXPECTED_ARG:
        {
            printf("Expected argument: %s |<- here\n", opt_out.opt);
            return 0;
        }

        case INVALID_OPTSTR:
        {
            printf("Error, invalid optstr value!\n");
            return 0;
        }

        case ARGV_END:
        default:
        {
            assert(0); // incorrect opt_flag
        }

    }
}

bool opt_proccessor (getopt_out opt_out, RUN_CONDITIONS *run_conds)
{
    if (!strcmp(opt_out.opt, "-s") || !strcmp(opt_out.opt, "--sorting"))
    {
        if (run_conds->sorts_n == 10)
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

        if (!strcmp(opt_out.optarg, "untouched"))
        {
            run_conds->sorts[run_conds->sorts_n - 1] = UNTOUCHED;
            return 1;
        }

        printf("Unknown sorting name: %s\n", opt_out.optarg);
        return 0;
    }

    if (!strcmp(opt_out.opt, "-i") || !strcmp(opt_out.opt, "--input_f"))
    {
        if (run_conds->input_file_selected == true)
        {
            printf("Can not select multiple files for input! (Do not use multiple -i, --input_f)\n");
            return 0;
        }

        if (strpbrk (opt_out.optarg, ":*?\"<>|") )
        {
            printf("\"%s\" is invalid file name\n", opt_out.optarg);
            return 0;
        }

        run_conds->input_file_selected = true;
        run_conds->input_file = opt_out.optarg;

        return 1;
    }

    if (!strcmp(opt_out.opt, "-o") || !strcmp(opt_out.opt, "--output_f"))
    {
        if (run_conds->output_file_selected == true)
        {
            printf("Can not select multiple files for output! (Do not use multiple -o, --output_f)\n");
            return 0;
        }

        if (strpbrk (opt_out.optarg, ":*?\"<>|") )
        {
            printf("\"%s\" is invalid file name\n", opt_out.optarg);
            return 0;
        }

        run_conds->output_file_selected = true;
        run_conds->output_file = opt_out.optarg;
        return 1;
    }

    printf("Option %s found in optstring but not in set_run_flags()", opt_out.opt);
            return 0;
}

void print_help()
{
    printf(
        "Use:\n"
        "-s <s_name>, --sorting <s_name> to sort according to s_name: beg_ascend, end_ascend, untouched (can use multiple)\n"
        "-i <filename>, --input-file <filename> to set input stream to filename\n"
        "-o <filename>, --output-file <filename> to set output stream to filename\n"
    );
}
