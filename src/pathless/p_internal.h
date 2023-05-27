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

struct IterationFile
{
	struct IterationFile* currentFileIterator;
	Directory* currentDirectory;
	int currentFileIndex;

	bool recursive;
	bool allowsDirs;
	
	uint includingCount;
	char** includingExtensions;
};

typedef struct IterationFile IterationFile;

bool pathless_path_is_dir(const char* const path);

#include <pathless.h>

#endif