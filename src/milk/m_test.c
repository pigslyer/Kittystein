#include "m_internal.h"

#define FLOOFY_EXPOSE_TEST_SUITE
#include <floofy.h>

#include <stdio.h>

static int testValues[] = {10, 20, 30, 40, 50, 60};

FLOOFY_TEST_REGISTER(linked_list_test)

void linked_list_test()
{
	int tempInt;
	int* tempIntPtr;
	LinkedListNode* tempNode;

	LinkedList* list = milk_linked_list_new(sizeof *testValues);

	tempInt = milk_linked_list_count(list);
	FLOOFY_TEST_ASSERT(tempInt == 0, "Linked list count of new list is %d, instead of 0!", tempInt);

	milk_linked_list_add_first(list, &testValues[0]);

	tempInt = milk_linked_list_count(list);
	FLOOFY_TEST_ASSERT(tempInt == 1, "Linked list count of list with 1 element is %d, instead of 1!", tempInt);

	tempNode = milk_linked_list_iterate(list);
	tempIntPtr = (int*)milk_linked_list_node_value(tempNode);
	FLOOFY_TEST_ASSERT(*tempIntPtr == testValues[0], "Linked list recorded value %d instead of %d!", tempInt, testValues[0]);

	*tempIntPtr = testValues[1];
	tempIntPtr = (int*) milk_linked_list_node_value(tempNode);
	FLOOFY_TEST_ASSERT(*tempIntPtr == testValues[1], "Linked list node value did not change despite setting pointer! Current: %d, Expected: %d!", *tempIntPtr, testValues[1]);
}