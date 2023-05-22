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
	uint 				lineNumber;
} FloofyTest;

extern uint		  		floofyFunctionCount;
extern FloofyTest 		floofyRegisteredFunctions[];

#include <floofy.h>
#endif