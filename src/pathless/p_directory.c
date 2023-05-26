#include "p_internal.h"

#include <dirent.h>
#include <milk.h>

Directory* pathless_directory_open(const char* const path)
{
	DIR* d = opendir(path);

	if (!d)
	{
		return null;
	}

	char* pathWithSlash;
	if (path[delight_string_length(path)] != '/')
	{
		pathWithSlash = delight_string_concat(path, "/");
	}
	else
	{
		pathWithSlash = delight_string_copy(path);
	}
	

	struct dirent* cur;
	LinkedList* list = milk_linked_list_new(sizeof(char*));

	char* curString;
	char* curName;
	while ((cur = readdir(d)) != null)
	{
		curName = cur->d_name;
		if (!delight_string_equals(curName, ".") && !delight_string_equals(curName, ".."))
		{
			curString = (char*) delight_string_concat(pathWithSlash, curName);
			milk_linked_list_add_first(list, &curString);
		}
	}

	Directory* ret = malloc(sizeof *ret);

	ret->pathToDir = delight_string_copy(path);
	ret->contains = (char**) milk_linked_list_to_array(list);
	ret->containsCount = milk_linked_list_count(list);

	milk_linked_list_free(list);
	free(pathWithSlash);

	return ret;
}

void pathless_directory_close(Directory* directory)
{
	uint containsCount = directory->containsCount;

	for (uint i = 0; i < containsCount; i++)
	{
		free(directory->contains[i]);
	}

	free(directory->contains);
	free(directory->pathToDir);
	free(directory);
}

const char* const * const pathless_directory_ls(Directory* directory)
{
	return (const char* const * const) directory->contains;
}