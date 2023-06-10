#ifndef PATHLESS_INTERNAL
#define PATHLESS_INTERNAL

#include <delight.h>

typedef struct
{
	char* 		pathToDir;
	char** 		contains;
	uint 		containsCount;
} Directory;

typedef struct IterationFile
{
	struct IterationFile* currentFileIterator;
	Directory* currentDirectory;
	int currentFileIndex;

	bool recursive;
	bool allowsDirs;
	
	uint includingCount;
	const char** includingExtensions;
} IterationFile;

typedef struct ConfigCategory
{
	char* categoryName;
	char** keys;
	char** values;
	uint pairCount;
	uint pairLength;
} ConfigCategory;

typedef struct ConfigFile
{
	ConfigCategory* categories;
	uint categoryCount;
	uint categoryLength;
} ConfigFile;


bool pathless_path_is_dir(const char* const path);

#include <pathless.h>

#endif