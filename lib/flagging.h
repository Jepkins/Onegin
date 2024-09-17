#ifndef FLAGGING_H
#define FLAGGING_H

enum GETOPT_RETS {
    ARGV_END =        -10,
    NOT_AN_OPT =       -1,
    OPT_UNKNOWN =      -2,
    EXPECTED_ARG =     -3,
    OPT_FOUND =         1,
    INVALID_OPTSTR = -100
};

struct getopt_out {
    char* opt;
    char* optarg;
    int optind;
};

enum SORTING {
    UNTOUCHED = 0,
    BEG_ASC = 1,
    END_ASC = 2
};

struct RUN_CONDITIONS {
    enum SORTING sorts[10];
    size_t sorts_n = 0;

    bool input_file_selected = 0;
    const char* input_file;

    bool output_file_selected = 0;
    const char* output_file;

    bool flagging_error;
};

bool run_setup(int argc, char** argv, RUN_CONDITIONS* run_conds);

GETOPT_RETS getopt_custom(int argc, char ** const argv, const char* optstring, getopt_out* opt_out);
GETOPT_RETS getoptarg(int argc, char ** const argv, getopt_out* opt_out);

bool set_run_flags(GETOPT_RETS opt_flag, getopt_out opt_out, RUN_CONDITIONS *run_conds);

bool opt_proccessor (getopt_out opt_out, RUN_CONDITIONS *run_conds);

void print_help();

bool normalizer_setup(int argc, char** argv, RUN_CONDITIONS* run_conds);

#endif // FLAGGING_H
