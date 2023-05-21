#ifndef PATHLESS_INTERNAL
#define PATHLESS_INTERNAL

#include <dirent.h>

typedef struct
{
	DIR* dir;
	struct dirent* contains;
} Directory;

typedef struct
{
	int a;
} IterationFile;

#include <pathless.h>

#endif