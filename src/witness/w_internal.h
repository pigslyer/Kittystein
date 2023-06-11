#ifndef WITNESS_INTERNAL
#define WITNESS_INTERNAL

#include <delight.h>

char* witness_input_get_line_base(void);

char* witness_input_get_line(void);

uint witness_input_get_command(char*** params);

#include <witness.h>
#endif