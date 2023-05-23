/*
* Delight is a standalone global module which defines multiple types and automatically imports stdlib.
*
* It also allows memory tracking if DELIGHT_MEMORY_LEAK_CHECK is defined as a compile time define.
* 
* Named by GonnaDoStuff.
* */

#ifndef DELIGHT_H
#define DELIGHT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef DELIGHT_INTERNAL

#endif

/// @brief Copies first size contents of sourceBuffer to newly allocated memory, returning the pointer to it. 
/// @param sourceBuffer The buffer we're copying from.
/// @param size The amount of that buffer we want to copy.
/// @return A pointer to the copied memory.
void* 		delight_memory_duplicate(const void* const sourceBuffer, size_t size);

/// @brief Copies first size content of sourceBuffer into destinationBuffer.
/// @param destinationBuffer The buffer we're copying to.
/// @param sourceBuffer The buffer we're copying from.
/// @param size The amount of source buffer we wish to copy.
void		delight_memory_copy(void* destinationBuffer, const void* const sourceBuffer, size_t size);

/// @brief Creates copy of string and returns a reference to the new string.
/// @param sourceString The string we wish to copy.
/// @return A pointer to the start of the copied string.
char* 		delight_string_copy(const char* const sourceString);

/// @brief Adds str2 to the end of str1, returning a pointer to the newly created string.
/// @param str1 The first string.
/// @param str2 The second string.
/// @return A reference to string1 + string2
char*		delight_string_concat(const char* const str1, const char* const str2);

/// @brief Appends contents of str2 to the end of str1. This function effectively automatically frees the "original" str1, as opposed to delight_string_concat.
/// @param str1 A pointer to the first string.
/// @param str2 The second string.
void		delight_string_append(char** str1, const char* const str2);

/// @brief Finds the length of a null terminated string.
/// @param string The string whose length we want.
/// @return The length of the string.
size_t 		delight_string_length(const char* const string);

/// @brief Compares 2 null terminated strings to see if they are equal
/// @param string1 The first string we wish to compare.
/// @param string2 The second string we wish to compare.
/// @return True if the contents of both strings are equal and they have the same length.
bool		delight_string_equals(const char* const string1, const char* const string2);


#ifdef DELIGHT_MEMORY_LEAK_CHECK



#endif

#define null NULL
typedef unsigned int uint;
typedef unsigned char byte;

#endif