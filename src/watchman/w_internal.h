#ifndef WATCHMAN_INTERNAL
#define WATCHMAN_INTERNAL
#define WATCHMAN_EXPOSE_VA_LIST

#include <slyertype.h>

void watchman_stream_sendline(char* buffer);

#include <watchman.h>
#endif