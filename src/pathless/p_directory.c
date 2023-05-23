#include "p_internal.h"

#include <dirent.h>
#include <milk.h>
#include <watchman.h>

Directory* pathless_directory_open(const char* const path)
{
	DIR* d = opendir(path);

	if (!d)
	{
		return null;
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
			curString = (char*) delight_string_copy(curName);
			milk_linked_list_add_first(list, &curString);
		}
	}

	Directory* ret = malloc(sizeof *ret);

	ret->pathToDir = delight_string_copy(path);
	ret->contains = (char**) milk_linked_list_to_array(list);
	ret->containsCount = milk_linked_list_count(list);

	milk_linked_list_free(list);

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