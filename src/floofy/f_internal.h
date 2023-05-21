#ifndef FLOOFY_INTERNAL
#define FLOOFY_INTERNAL

typedef void (*FloofyTestFunction)(void);

extern FloofyTestFunction registeredFunctions[];

#include <floofy.h>
#endif