#include "p_internal.h"

#include <dirent.h>
#include <watchman.h>

Directory* pathless_directory_open(const char* const path)
{
	char* pathWithSlash;
	char* pathWithoutSlash;
	if (path[string_length(path)] != '/')
	{
		pathWithSlash = string_concat(path, "/");
		pathWithoutSlash = string_copy(path);
	}
	else
	{
		pathWithSlash = string_copy(path);

		pathWithoutSlash = string_copy(path);
		pathWithoutSlash[string_length(pathWithoutSlash)] = '\0';
	}

	DIR* d = opendir(pathWithoutSlash);

	if (!d)
	{
		free(pathWithSlash);
		free(pathWithoutSlash);
		return null;
	}

	struct dirent* cur;

	char* curName;

	const u32 maxCount = 1024;

	char* folderContains[maxCount];
	u32 count = 0;

	while ((cur = readdir(d)) != null && count < maxCount)
	{
		curName = cur->d_name;
		if (!string_equals(curName, ".") && !string_equals(curName, ".."))
		{
			folderContains[count++] = (char*) string_concat(pathWithSlash, curName);
		}
	}

	Directory* ret = malloc(sizeof *ret);

	ret->pathToDir = string_copy(path);
	ret->contains = memory_duplicate(folderContains, count * sizeof *folderContains);;
	ret->containsCount = count;

	free(pathWithSlash);
	free(pathWithoutSlash);

	return ret;
}

void pathless_directory_close(Directory* directory)
{
	u32 containsCount = directory->containsCount;

	for (u32 i = 0; i < containsCount; i++)
	{
		free(directory->contains[i]);
	}

	free(directory->contains);
	free(directory->pathToDir);
	free(directory);
}

const char* const * const pathless_directory_ls(Directory* directory, u32* containsCount)
{
	*containsCount = directory->containsCount;
	return (const char* const * const) directory->contains;
}