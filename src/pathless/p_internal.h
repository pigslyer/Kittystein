#ifndef PATHLESS_INTERNAL
#define PATHLESS_INTERNAL

#include <dirent.h>

#include <delight.h>

typedef struct
{
	char* 		pathToDir;
	char** 		contains;
	uint 		containsCount;
} Directory;

typedef struct
{
	int a;
} IterationFile;

#include <pathless.h>

#endif