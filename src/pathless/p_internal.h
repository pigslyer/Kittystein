#ifndef PATHLESS_INTERNAL
#define PATHLESS_INTERNAL

#include <delight.h>

typedef struct
{
	char* 		pathToDir;
	char** 		contains;
	u32 		containsCount;
} Directory;

typedef struct IterationFile
{
	struct IterationFile* currentFileIterator;
	Directory* currentDirectory;
	int currentFileIndex;

	bool recursive;
	bool allowsDirs;
	
	u32 includingCount;
	const char** includingExtensions;
} IterationFile;

typedef struct ConfigCategory
{
	char* categoryName;
	char** keys;
	char** values;
	u32 pairCount;
	u32 pairLength;
} ConfigCategory;

typedef struct ConfigFile
{
	ConfigCategory* categories;
	u32 categoryCount;
	u32 categoryLength;
} ConfigFile;


bool pathless_path_is_dir(const char* const path);

#include <pathless.h>

#endif