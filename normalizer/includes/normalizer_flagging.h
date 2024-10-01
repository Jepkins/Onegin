#ifndef NORMALIZER_FLAGGING_H
#define NORMALIZER_FLAGGING_H

#include "flagging.h"

static const char DEFAULT_INPUT_FILE[] = "data/text_rus.txt";
static const char DEFAULT_OUTPUT_FILE[] = "data/text_rus_norm.txt";

typedef enum {
    DEFAULT = 0,
    CP1251  = 1,
    UTF8    = 2
} Encoding;

typedef struct {
    Encoding encoding = DEFAULT;

    bool is_input_file_selected = false;
    const char* input_file = nullptr;

    bool is_output_file_selected = false;
    const char* output_file = nullptr;

    bool flagging_error;
} StartConfig;

bool normalizer_setup(int argc, char** argv, StartConfig* run_conds);
bool normalizer_opt_proccessor (getopt_out opt_out, StartConfig *run_conds);

#endif // NORMALIZER_FLAGGING_H
