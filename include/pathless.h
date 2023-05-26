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


IterationFile* 					pathless_iterate_begin(const char* const path, bool recursive, bool allowDirs, char** excludeExtensions, uint exclusionCount);
void 							pathless_iterate_end(IterationFile* state);

const char* const				pathless_iterate_get_current(IterationFile* state);
bool 							pathless_iterate_next(IterationFile* state);

Directory* 						pathless_directory_open(const char* const path);
void 							pathless_directory_close(Directory* directory);

const char* const * const		pathless_directory_ls(Directory* directory);

#endif