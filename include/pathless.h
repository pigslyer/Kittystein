/*
* Pathless is a module dependant on Milk which implements various file I/O operations.
* 
* 
* 
* Named after the Pathless (https://store.steampowered.com/app/1492680/The_Pathless/)
* */

#ifndef PATHLESS_H
#define PATHLESS_H

#include <delight.h>

#ifndef PATHLESS_INTERNAL

typedef void IterationFile;
typedef void Directory;

#endif

/// @brief Begins iterating across directory, listing every file or folder as specified by these parameters. The iteration file starts before the first element, so pathless_iterate_next should be called in order to move it to the next element.
/// @param path The relative or absolute path to the directory we wish to open. May end with /.
/// @param recursive Whether or not we want to iterate into subdirectories.
/// @param allowDirs Whether or not current should also return directories.
/// @param onlyExtensions Files with which extensions should be returned. If all files should be permitted, pass {""}.
/// @param extensionCount The number of file extensions in the onlyExtensions array. 0 means that no files will be permitted.
/// @return An object which allows iterating over file.
IterationFile* 					pathless_iterate_begin(const char* const path, bool recursive, bool allowDirs, const char** onlyExtensions, uint extensionCount);

/// @brief End iteration across directory, freeing this iteration file.
/// @param file Which iteration file should be ended. 
void 							pathless_iterate_end(IterationFile* file);

/// @brief Returns the current file this IterationFile is pointing to. This function has no effect on the state of the iteration file.
/// @param file The current IterationFile.
/// @return The file it is pointing to.
const char* const				pathless_iterate_get_current(IterationFile* file);

/// @brief If there is another file this iteration file could iterate to, returns true and updates the return of pathless_iterate_get_current. Otherwise returns false.
/// @param file The current IterationFile.
/// @return True if there are more files to iterate to.
bool 							pathless_iterate_next(IterationFile* file);

/// @brief Opens the directory pointed to by path. ALL filesystem operations related to opening a directory are performed by this function.
/// @param path The relative or absolute path to a directory.
/// @return The directory reference object.
Directory* 						pathless_directory_open(const char* const path);

/// @brief Closes the opened directory, freeing associated memory.
/// @param directory The directory we wish to close.
void 							pathless_directory_close(Directory* directory);

// should probably add a FileData thing instead of this to allow more easily reading stuff. probably adding it in the next general IO push

/// @brief Returns an array of fileCount files, which are directly contained within this directory. The paths are of the format "directoryPath/file", where directoryPath is the path given to pathless_directory_open and file is the full file extension.
/// @param directory The directory we want to open.
/// @param fileCount The number of files in the returned an array.
/// @return The internal array of files this directory contains. This should NOT be freed.
const char* const * const		pathless_directory_ls(Directory* directory, uint* fileCount);


char*							pathless_file_read_as_text(const char* const path, size_t* length);

void							pathless_file_write_as_text(const char* const path, char* buffer, size_t length);

#endif