#include "p_internal.h"

#include <dirent.h>
#include <milk.h>

Directory* pathless_directory_open(char* path)
{
	DIR* d = opendir(path);

	if (!d)
	{
		return null;
	}

	Directory* ret = malloc(sizeof *ret);
	struct dirent* cur;

	LinkedList* list = milk_linked_list_new(sizeof *cur);

	while ((cur = readdir(d)) != null)
	{
		milk_linked_list_add_first(list, cur);
	}

	return null;
}

void pathless_directory_close(Directory* directory)
{
	closedir(directory->dir);
}