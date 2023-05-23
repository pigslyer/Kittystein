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


IterationFile* pathless_iterate_begin(const char* const path, bool recursive, char** excludeExtensions);
void pathless_iterate_end(IterationFile* state);

char* pathless_iterate_get_path(IterationFile* state);
bool pathless_iterate_has_next(IterationFile* state);
bool pathless_iterate_is_directory(IterationFile* state);

Directory* pathless_directory_open(const char* const path);
void pathless_directory_close(Directory* directory);

#endif