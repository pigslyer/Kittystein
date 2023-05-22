#include "m_internal.h"

#include <string.h>

LinkedList* milk_linked_list_new(size_t elementSize)
{
	LinkedList* ret = (LinkedList*) malloc(sizeof *ret);
	ret->first = null;
	ret->count = 0;
	ret->elementSize = elementSize;
	return ret;
}

void milk_linked_list_add_first(LinkedList* list, void* element)
{
	LinkedListNode* newNode = malloc(sizeof *newNode);
	newNode->value = milk_util_shallow_copy_new(element, list->elementSize);
	newNode->next = list->first;
	list->first = newNode;
	list->count += 1;
}

void milk_linked_list_free(LinkedList* list)
{
	LinkedListNode* cur = list->first;
	LinkedListNode* next;

	while (cur != null)
	{
		milk_util_shallow_copy_free(cur->value);
		next = cur->next;
		free(cur);
		cur = next;
	}

	free(list);
}

uint milk_linked_list_count(LinkedList* list)
{
	return list->count;
}

void* milk_linked_list_to_array(LinkedList* list)
{
	size_t incAmount = list->elementSize;

	byte* ret = (byte*) malloc(list->count * list->elementSize);
	byte* temp = ret;

	for (LinkedListNode* cur = list->first; cur != null; cur = cur->next)
	{
		memcpy(temp, cur->value, incAmount);
		temp += incAmount;
	}

	return ret;
}

inline LinkedListNode* milk_linked_list_iterate(LinkedList* list)
{
	return list->first;
}

inline void* milk_linked_list_node_value(LinkedListNode* node)
{
	return node->value;
}

inline LinkedListNode* milk_linked_list_node_next(LinkedListNode* node)
{
	return node->next;
}