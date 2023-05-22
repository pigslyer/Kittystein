#ifndef PATHLESS_INTERNAL
#define PATHLESS_INTERNAL

#include <dirent.h>

#include <delight.h>

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