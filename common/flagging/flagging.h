#ifndef FLAGGING_H
#define FLAGGING_H

#include "text_t.h"

static const char DEFAULT_INPUT_FILE[] = "data/text_rus_norm.txt";
static const char DEFAULT_OUTPUT_FILE[] = "data/output.txt";
static const size_t MAX_N_SORTS = 10;

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

bool run_setup(int argc, char** argv, StartConfig* run_conds);

GetoptResult getopt_custom(int argc, char ** const argv, const char* optstring, getopt_out* opt_out);
GetoptResult getoptarg(int argc, char ** const argv, getopt_out* opt_out);

bool set_run_flags(GetoptResult opt_flag, getopt_out opt_out, StartConfig *run_conds);

bool opt_proccessor (getopt_out opt_out, StartConfig *run_conds);

void print_help();

bool normalizer_setup(int argc, char** argv, StartConfig* run_conds);

#endif // FLAGGING_H
