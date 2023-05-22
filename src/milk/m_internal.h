#ifndef MILK_INTERNAL
#define MILK_INTERNAL

#include <delight.h>

void* milk_util_shallow_copy_new(void* what, size_t length);
void milk_util_shallow_copy_free(void* what);


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