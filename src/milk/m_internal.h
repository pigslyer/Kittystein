#ifndef MILK_INTERNAL
#define MILK_INTERNAL

#include <delight.h>


struct LinkedListNode
{
	struct LinkedListNode* next;
	void* value;
};

typedef struct LinkedListNode LinkedListNode;

typedef struct
{
	uint count;
	size_t elementSize;
	LinkedListNode* first;
} LinkedList;


#include <milk.h>

#endif