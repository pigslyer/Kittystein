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


bool delight_memory_equals(const void* buffer1, const void* buffer2, const size_t size)
{
	return memcmp(buffer1, buffer2, size) == 0;
}

static const size_t usedMemoryCountIncrement = 1024;
static MemoryAllocation* usedMemory;

static size_t usedMemoryLength;
static ulong usedMemoryCount;

static size_t allocatedMemory;


static void delight_memory_used_memory_resize(void);


#define DELIGHT_MEMORY_DEBUG_TOOLS
#define DELIGHT_MEMORY_TRACKER
#include <delight.h>

void delight_memory_usage_report(ulong* allocationCount, size_t* totalAllocatedMemory)
{	
	if (allocationCount)
	{
		*allocationCount = usedMemoryCount;
	}

	if (totalAllocatedMemory)
	{
		*totalAllocatedMemory = allocatedMemory;
	}
}


void delight_memory_usage_request_location(const ulong allocation, const char * * const file, uint* line)
{
	*file = usedMemory[allocation].requestLocation;
	*line = usedMemory[allocation].requestLine;
}

size_t delight_memory_usage_allocation_amount(const ulong allocation)
{
	return usedMemory[allocation].allocationAmount;
}

void* delight_memory_usage_allocation_location(const ulong allocation)
{
	return usedMemory[allocation].allocationLocation;
}

// currently none of this is thread safe!

// we need to actually perform the underlying functions eventually
#undef malloc
#undef realloc
#undef free
#undef calloc

// since string allocations etc. might still be using DELIGHT_MEMORY_LEAK_TOOLS, their allocations would still call delight's memory functions. delight memory's internal
// memory should also be tracked. these macros act as the now undef-ed original DELIGHT_MEMORY_LEAK_TOOLS macros and should be kept in sync.
#define malloc_internal(size)					_delight_memory_malloc(size, _delight_string_make_relative_to_src(__FILE__), __LINE__)
#define realloc_internal(orgBuffer, size)		_delight_memory_realloc(orgBuffer, size, _delight_string_make_relative_to_src(__FILE__), __LINE__)
#define free_internal(what)						_delight_memory_free(what, _delight_string_make_relative_to_src(__FILE__), __LINE__)

void* _delight_memory_malloc(const size_t amount, const char* const file, const uint line)
{
	void* ret = malloc(amount);

	if (ret == null)
	{
		return null;
	}

	if ((size_t)(usedMemoryCount) >= usedMemoryLength)
	{
		delight_memory_used_memory_resize();
	}

	
	MemoryAllocation* cur = &usedMemory[usedMemoryCount++];

	cur->requestLocation = delight_string_copy(file);
	cur->requestLine = line;
	cur->allocationAmount = amount;
	cur->allocationLocation = ret;

	allocatedMemory += amount;

	return ret;
}

void* _delight_memory_calloc(const size_t elementCount, const size_t elementSize, const char* const file, const uint line)
{
	void* ret = calloc(elementCount, elementSize);

	if (ret == null)
	{
		return null;
	}

	if ((size_t)(usedMemoryCount) >= usedMemoryLength)
	{
		delight_memory_used_memory_resize();
	}

	MemoryAllocation* cur = &usedMemory[usedMemoryCount++];

	cur->requestLocation = delight_string_copy(file);
	cur->requestLine = line;
	cur->allocationAmount = elementCount * elementSize;
	cur->allocationLocation = ret;

	allocatedMemory += cur->allocationAmount;

	return ret;
}


void* _delight_memory_realloc(void* orgBuffer, const size_t amount, const char* const file, const uint line)
{
	void* ret = realloc(orgBuffer, amount);
	
	if (ret == null)
	{
		return null;
	}

	MemoryAllocation* cur;
	for (ulong i = 0; i < usedMemoryCount; i++)
	{
		cur = &usedMemory[i];
		if (orgBuffer == cur->allocationLocation)
		{
			allocatedMemory = allocatedMemory - cur->allocationAmount + amount;

			cur->allocationLocation = ret;
			
			free_internal(cur->requestLocation);
			cur->requestLocation = delight_string_copy(file);
			cur->requestLine = line;
			cur->allocationAmount = amount;

			return ret;
		}
	}

	// what the fuck could've happened?

	return null;
}

void _delight_memory_free(void* what, const char* const file, const uint line)
{
	MemoryAllocation* cur;
	for (ulong i = 0; i < usedMemoryCount; i++)
	{
		cur = &usedMemory[i];
		if (what == cur->allocationLocation)
		{
			allocatedMemory -= cur->allocationAmount;

			free_internal(cur->requestLocation);

			usedMemory[i] = usedMemory[--usedMemoryCount];
			
			free(what);
			return;
		}
	}

	// some kind of reporting that an illegal free was attempted?
}

void delight_memory_used_memory_resize()
{
	if (usedMemory == null)
	{
		usedMemoryLength = usedMemoryCountIncrement;
		// calling delight's malloc here would result in an infinite loop of allocating the starting buffer.
		usedMemory = malloc(usedMemoryCountIncrement * sizeof(MemoryAllocation));
	}
	else
	{
		usedMemoryLength += usedMemoryCountIncrement;
		// calling the internal realloc here wouldn't make sense as usedMemory wasn't initialized by delight's malloc
		usedMemory = realloc(usedMemory, usedMemoryLength * sizeof(MemoryAllocation));
	}
}