#ifndef FLOOFY_INTERNAL

#define FLOOFY_INTERNAL
#define FLOOFY_EXPOSE_TEST_SUITE

typedef void (*FloofyTestFunction)(void);

extern FloofyTestFunction registeredFunctions[];

#include <floofy.h>
#endif