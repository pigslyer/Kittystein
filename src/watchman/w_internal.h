#ifndef WATCHMAN_INTERNAL
#define WATCHMAN_INTERNAL
#define WATCHMAN_EXPOSE_VA_LIST

#include <delight.h>

typedef void (*WatchmanStreamCallback)(char*);

void watchman_stream_push(char* message);

#include <watchman.h>
#endif