#include "d_internal.h"
#include <string.h>

inline void* delight_memory_duplicate(const void* const sourceBuffer, size_t size)
{
	void* ret = malloc(size);
	delight_memory_copy(ret, sourceBuffer, size);
	return ret;
}

inline void delight_memory_copy(void* destinationBuffer, const void* const sourceBuffer, size_t size)
{
	memcpy(destinationBuffer, sourceBuffer, size);
}