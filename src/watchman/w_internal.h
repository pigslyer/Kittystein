#ifndef WATCHMAN_INTERNAL
#define WATCHMAN_INTERNAL

#include <slyertype.h>

void watchman_stream_sendline(char* buffer);

// CONSTANTS
extern const char* const LOG_FOLDER;

#include <watchman.h>
#endif