#ifndef WITNESS_INTERNAL
#define WITNESS_INTERNAL

#include <delight.h>
#include <witness.h>

typedef struct
{
	witness_function function;
	const char* functionName;
} witness_function_data;


extern witness_function_data* functions;
extern u32 functionCount;


#endif