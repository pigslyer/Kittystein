#include "p_internal.h"


IterationFile* pathless_iterate_begin(const char* const path, bool recursive, bool allowDirs, const char** onlyExtensions, uint includeCount)
{
	

	IterationFile* ret = malloc(sizeof(IterationFile));

	ret->currentFileIterator = null;
	ret->currentDirectory = pathless_directory_open(path);
	ret->currentFileIndex = -1;

	ret->recursive = recursive;
	ret->allowsDirs = allowDirs;

	ret->includingCount = includeCount;
	ret->includingExtensions = includeCount > 0 ? delight_memory_duplicate(onlyExtensions, includeCount * sizeof *onlyExtensions) : null;
	
	return ret;
}

void pathless_iterate_end(IterationFile* file)
{
	if (file->currentFileIterator)
	{
		pathless_iterate_end(file);
	}

	if (file->includingExtensions)
	{
		free(file->includingExtensions);
	}

	pathless_directory_close(file->currentDirectory);
	free(file);
}


const char* const pathless_iterate_get_current(IterationFile* file)
{
	if (file->currentFileIndex < 0 || file->currentFileIndex >= file->currentDirectory->containsCount)
	{
		return null;
	}

	if (file->currentFileIterator)
	{
		return pathless_iterate_get_current(file->currentFileIterator);
	}

	return file->currentDirectory->contains[file->currentFileIndex];
}

bool pathless_iterate_next(IterationFile* file)
{
	const char* curPath;

	uint containsCount;
	const char* const * contains = pathless_directory_ls(file->currentDirectory, &containsCount);

	// if we're currently iterating a subdirectory, check if that one's done
	if (file->currentFileIterator)
	{
		if (pathless_iterate_next(file->currentFileIterator))
		{
			return true;
		}

		pathless_iterate_end(file->currentFileIterator);
		file->currentFileIterator = null;
	}
	// if we're allowing dirs we may have moved to a directory in the last step and not yet opened it to signify we have to move through it too
	else if (file->currentFileIndex >= 0 && file->recursive && file->allowsDirs)
	{
		curPath = contains[file->currentFileIndex];

		if (pathless_path_is_dir(curPath))
		{
			file->currentFileIterator = pathless_iterate_begin(curPath, true, true, file->includingExtensions, file->includingCount);
			
			if (pathless_iterate_next(file->currentFileIterator))
			{
				return true;
			}

			pathless_iterate_end(file->currentFileIterator);
			file->currentFileIterator = null;
		}
	}
	
	file->currentFileIndex += 1;

	// there are no more files in this subfolder
	if (!(file->currentFileIndex < containsCount))
	{
		return false;
	}

	curPath = contains[file->currentFileIndex];

	if (pathless_path_is_dir(curPath))
	{
		if (file->allowsDirs)
		{
			return true;
		}

		if (file->recursive)
		{
			file->currentFileIterator = pathless_iterate_begin(curPath, true, false, file->includingExtensions, file->includingCount);
			
			if (pathless_iterate_next(file->currentFileIterator))
			{
				return true;
			}

			pathless_iterate_end(file->currentFileIterator);
			file->currentFileIterator = null;
		}
	}
	else
	{
		for (uint i = 0; i < file->includingCount; i++)
		{
			if (delight_string_ends_with(curPath, file->includingExtensions[i]))
			{
				return true;
			}
		}
	}

	return pathless_iterate_next(file);

}
