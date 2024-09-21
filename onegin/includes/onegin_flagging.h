#ifndef ONEGIN_FLAGGING_H
#define ONEGIN_FLAGGING_H

#include "flagging.h"

static const char DEFAULT_INPUT_FILE[] = "data/text_rus_norm.txt";
static const char DEFAULT_OUTPUT_FILE[] = "data/output.txt";
static const size_t MAX_N_SORTS = 10;

bool onegin_setup(int argc, char** argv, StartConfig* run_conds);
bool onegin_opt_proccessor (getopt_out opt_out, StartConfig *run_conds);

#endif // ONEGIN_FLAGGING_H
