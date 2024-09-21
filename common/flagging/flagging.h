#ifndef FLAGGING_H
#define FLAGGING_H

#include "text_t.h"

typedef enum {
    ARGV_END =        -10,
    NOT_AN_OPT =       -1,
    OPT_UNKNOWN =      -2,
    EXPECTED_ARG =     -3,
    OPT_FOUND =         1,
    INVALID_OPTSTR = -100
} GetoptResult;

typedef struct {
    char* opt;
    char* optarg;
    int optind;
} getopt_out;

typedef struct {
    Sorting sorts[10] = {};
    size_t sorts_n = 0;

    bool is_input_file_selected = false;
    const char* input_file = nullptr;

    bool is_output_file_selected = false;
    const char* output_file = nullptr;

    bool flagging_error;
} StartConfig;

typedef bool (*opt_proccessor_t) (getopt_out opt_out, StartConfig *run_conds);

GetoptResult getopt_custom(int argc, char ** const argv, const char* optstring, getopt_out* opt_out);
GetoptResult getoptarg(int argc, char ** const argv, getopt_out* opt_out);

bool check_opt_flag(GetoptResult opt_flag, getopt_out opt_out);

#endif // FLAGGING_H
