#ifndef DELIGHT_INTERNAL
#define DELIGHT_INTERNAL

#include <stdint.h>

typedef struct
{
	void* allocationLocation;
	uint64_t allocationAmount;
	char* requestLocation;
	unsigned int requestLine;
} MemoryAllocation;

typedef struct
{
	// capacity and count are in elements, not bytes!
    uint64_t capacity;
    uint64_t count;
    uint64_t elemSize;
    // this then continues with the array itself 
} DArray;

#include <delight.h>

#endif