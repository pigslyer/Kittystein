#ifndef DELIGHT_INTERNAL
#define DELIGHT_INTERNAL

#include <stdint.h>

typedef struct
{
	void* allocationLocation;
	size_t allocationAmount;
	char* requestLocation;
	unsigned int requestLine;
} MemoryAllocation;

#include <delight.h>

#endif