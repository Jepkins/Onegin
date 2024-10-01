#ifndef ONEGIN_FLAGGING_H
#define ONEGIN_FLAGGING_H

#include "text_t.h"
#include "flagging.h"

static const char DEFAULT_INPUT_FILE[] = "data/text_rus_norm.txt";
static const char DEFAULT_OUTPUT_FILE[] = "data/output.txt";
static const size_t MAX_N_SORTS = 10;

typedef enum {
    DEFAULT = 0, // WORK
    CP1251  = 1, // WORK
    UTF8    = 2
} Encoding;

typedef struct {
    Encoding encoding = DEFAULT;

    Sorting sorts[10] = {};
    size_t sorts_n = 0;

    bool is_input_file_selected = false;
    const char* input_file = nullptr;

    bool is_output_file_selected = false;
    const char* output_file = nullptr;

    bool flagging_error = false;
} StartConfig;

bool onegin_setup(int argc, char** argv, StartConfig* run_conds);
bool onegin_opt_proccessor (getopt_out opt_out, StartConfig *run_conds);

#endif // ONEGIN_FLAGGING_H
