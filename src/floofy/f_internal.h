#ifndef FLOOFY_INTERNAL

#define FLOOFY_INTERNAL
#define FLOOFY_EXPOSE_TEST_SUITE

#include <delight.h>

typedef void (*FloofyTestFunction)(void);

typedef struct
{
	FloofyTestFunction 	functionCallback;
	const char*			functionName;
	const char* 		filePath;
	u32 				lineNumber;
} FloofyTest;

extern u32		  		floofyFunctionCount;
extern FloofyTest 		floofyRegisteredFunctions[];

#include <floofy.h>
#endif