#include "p_internal.h"


IterationFile* pathless_iterate_begin(const char* const path, bool recursive, bool allowDirs, char** excludeExtensions, uint excludeCount)
{
	IterationFile* ret = malloc(sizeof(IterationFile));

	ret->currentFileIterator = null;
	ret->currentDirectory = pathless_directory_open(path);
	ret->currentFileIndex = -1;

	ret->recursive = recursive;
	ret->allowsDirs = allowDirs;

	ret->excludingCount = excludeCount;
	ret->excludingExtensions = delight_memory_duplicate(excludeExtensions, excludeCount * sizeof *excludeExtensions);

	return ret;
}

void pathless_iterate_end(IterationFile* file)
{
	if (file->currentFileIterator)
	{
		pathless_iterate_end(file);
	}

	pathless_directory_close(file->currentDirectory);
	free(file->excludingExtensions);
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
	Directory* curDir = file->currentDirectory;
	const char* curPath;

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
	else if (file->recursive && file->allowsDirs)
	{
		curPath = curDir->contains[file->currentFileIndex];

		if (pathless_path_is_dir(curPath))
		{
			file->currentFileIterator = pathless_iterate_begin(curPath, true, true, file->excludingExtensions, file->excludingCount);
			
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
	if (!(file->currentFileIndex < curDir->containsCount))
	{
		return false;
	}

	curPath = curDir->contains[file->currentFileIndex];

	if (file->recursive && pathless_path_is_dir(curPath))
	{
		// if we're allowing subdirs then this subdir is enough
		if (file->allowsDirs)
		{
			return true;
		}

		file->currentFileIterator = pathless_iterate_begin(curPath, true, false, file->excludingExtensions, file->excludingCount);
		
		if (pathless_iterate_next(file->currentFileIterator))
		{
			return true;
		}

		pathless_iterate_end(file->currentFileIterator);
		file->currentFileIterator = null;
	}

	for (uint i = 0; i < file->excludingCount; i++)
	{
		if (delight_string_ends_with(curPath, file->excludingExtensions[i]))
		{
			return true;
		}
	}

	return pathless_iterate_next(file);
}
