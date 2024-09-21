#ifndef NORMALIZER_FLAGGING_H
#define NORMALIZER_FLAGGING_H

#include "flagging.h"

static const char DEFAULT_INPUT_FILE[] = "data/text_rus.txt";
static const char DEFAULT_OUTPUT_FILE[] = "data/text_rus_norm.txt";

bool normalizer_setup(int argc, char** argv, StartConfig* run_conds);
bool normalizer_opt_proccessor (getopt_out opt_out, StartConfig *run_conds);

#endif // NORMALIZER_FLAGGING_H
