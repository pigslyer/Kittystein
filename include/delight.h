/*
* Delight is a standalone global module which defines multiple types and automatically imports stdlib.
*
* It also allows memory tracking if DELIGHT_MEMORY_TRACKER is defined as a compiler define. In order to get
* diagnostics, include delight with DELIGHT_MEMORY_DEBUG_TOOLS.
* 
* Named by GonnaDoStuff.
* */

#ifndef DELIGHT_H
#define DELIGHT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define null NULL
typedef uint64_t ulong;
typedef unsigned int uint;
typedef unsigned char byte;

typedef enum
{
	UsageReqNever,
	UsageReqOptional,
	UsageReqRequired,
} UsageReq;

#ifndef DELIGHT_INTERNAL

#endif

/// @brief Copies first size contents of sourceBuffer to newly allocated memory, returning the pointer to it. 
/// @param sourceBuffer The buffer we're copying from.
/// @param size The amount of that buffer we want to copy.
/// @return A pointer to the copied memory.
void* 					delight_memory_duplicate(const void* const sourceBuffer, size_t size);

/// @brief Copies first size content of sourceBuffer into destinationBuffer.
/// @param destinationBuffer The buffer we're copying to.
/// @param sourceBuffer The buffer we're copying from.
/// @param size The amount of source buffer we wish to copy.
void					delight_memory_copy(void* destinationBuffer, const void* const sourceBuffer, const size_t size);

/// @brief Checks if the contents of buffer1 equal the contents of buffer2 for a distance of size.
/// @param buffer1 The first buffer we're comparing.
/// @param buffer2 The second buffer we're comparing.
/// @param size The length along both buffers that we're checking equality for.
/// @return True if the buffers are equal for length size.
bool					delight_memory_equals(const void* buffer1, const void* buffer2, size_t size);


/// @brief Creates copy of string and returns a reference to the new string.
/// @param sourceString The string we wish to copy.
/// @return A pointer to the start of the copied string.
char* 					delight_string_copy(const char* const sourceString);

/// @brief Adds str2 to the end of str1, returning a pointer to the newly created string.
/// @param str1 The first string.
/// @param str2 The second string.
/// @return A reference to string1 + string2
char*					delight_string_concat(const char* const str1, const char* const str2);

/// @brief Appends contents of str2 to the end of str1. This function effectively automatically frees the "original" str1, as opposed to delight_string_concat.
/// @param str1 A pointer to the first string.
/// @param str2 The second string.
void					delight_string_append(char** str1, const char* const str2);

/// @brief Finds the length of a null terminated string.
/// @param string The string whose length we want.
/// @return The length of the string.
size_t 					delight_string_length(const char* const string);

/// @brief Compares 2 null terminated strings to see if they are equal
/// @param string1 The first string we wish to compare.
/// @param string2 The second string we wish to compare.
/// @return True if the contents of both strings are equal and they have the same length.
bool					delight_string_equals(const char* const string1, const char* const string2);

bool					delight_string_ends_with(const char* const string, const char* const suffix);

size_t					delight_string_index_of_char(const char* const string, char value, size_t from);

char*					delight_string_substring(const char* const string, size_t from, size_t length);

char**					delight_string_split(const char* const string, char byChar, uint* splitCount);


// tried making a macro for calling this, C didn't like it.
/// @brief Returns the character in the passed string where "src" first occurs or null if the string is too short or doesn't contain "src". The string returned by this function should not be freed, rather the passed argument should be.
/// @param string The filepath, probably aquired from __FILE__.
/// @return A pointer to the character in the string where "src" begins or NULL on failure.
char*					_delight_string_make_relative_to_src(char* string);


bool					delight_enum_usage_req_eval(UsageReq req, bool value);

char*					delight_convert_int_to_string(int val);

char*					delight_convert_bool_to_string(bool val);

int						delight_convert_string_to_int(char* str);

bool					delight_convert_string_to_bool(char* str);


#endif



// enabling places macros over malloc, realloc, calloc and free. these should always be defined on the compiler level!
#ifdef DELIGHT_MEMORY_TRACKER

void*			_delight_memory_malloc(const size_t amount, char* file, uint line);
void*			_delight_memory_calloc(const size_t elementCount, const size_t elementSize, char* file, uint line);
void*			_delight_memory_realloc(void* orgBuffer, const size_t amount, char* file, uint line);
void			_delight_memory_free(void* what, char* const file, const uint line);

#define malloc(size) 				_delight_memory_malloc(size, _delight_string_make_relative_to_src(__FILE__), __LINE__)
#define calloc(elementCount, size)	_delight_memory_calloc(elementCount, size, _delight_string_make_relative_to_src(__FILE__), __LINE__)
#define realloc(orgBuffer, size) 	_delight_memory_realloc(orgBuffer, size, _delight_string_make_relative_to_src(__FILE__), __LINE__)
#define free(what) 					_delight_memory_free(what, _delight_string_make_relative_to_src(__FILE__), __LINE__)

#endif


// this is outside so that we can still write code to use debug tools even if DELIGHT_MEMORY_TRACKER isn't defined
#ifdef DELIGHT_MEMORY_DEBUG_TOOLS

#ifndef _DELIGHT_MEMORY_DEBUG_TOOLS
/// @brief Passes along an array to every MemoryAllocation, alongside the current total allocated memory.
/// @param allocationCount The number of active allocations.
/// @param totalAllocatedMemory The total amount of memory allocated by all functions.
void				delight_memory_usage_report(ulong* allocationCount, size_t* totalAllocatedMemory);

/// @brief Requests the location where a certain block of memory was allocated.
/// @param allocation The allocation we're querying.
/// @param file The file from which the allocation was made.
/// @param line The line on which the allocation was made.
void				delight_memory_usage_request_location(const ulong allocation, const char* * const file, uint* line);

/// @brief Requests the amount of memory this allocation takes.
/// @param allocation The allocation we're querying.
/// @return The amount of space that allocation takes.
size_t				delight_memory_usage_allocation_amount(const ulong allocation);

/// @brief Requests the location in memory of the current allocation. Using this memory is obviously dangerous.
/// @param allocation The allocation we're querying.
/// @return The location in memory where this allocation is located.
void*				delight_memory_usage_allocation_location(const ulong allocation);

#endif

#endif
