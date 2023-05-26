#include "m_internal.h"

#define FLOOFY_EXPOSE_TEST_SUITE
#include <floofy.h>

FLOOFY_TEST_REGISTER(milk_linked_list_test, true)

void milk_linked_list_test()
{
	int testValues[] = {10, 20, 30, 40, 50, 60};
	
	int tempInt;
	int* tempIntPtr;
	LinkedListNode* tempNode;

	LinkedList* list = milk_linked_list_new(sizeof *testValues);

	tempInt = milk_linked_list_count(list);
	FLOOFY_TEST_ASSERT_ARG(tempInt == 0, "Linked list count of new list is %d, instead of 0!", tempInt);

	milk_linked_list_add_first(list, &testValues[0]);

	tempInt = milk_linked_list_count(list);
	FLOOFY_TEST_ASSERT_ARG(tempInt == 1, "Linked list count of list with 1 element is %d, instead of 1!", tempInt);

	tempNode = milk_linked_list_iterate(list);
	tempIntPtr = (int*)milk_linked_list_node_value(tempNode);
	FLOOFY_TEST_ASSERT_ARG(*tempIntPtr == testValues[0], "Linked list recorded value %d instead of %d!", tempInt, testValues[0]);

	*tempIntPtr = testValues[1];
	tempIntPtr = (int*) milk_linked_list_node_value(tempNode);
	FLOOFY_TEST_ASSERT_ARG(*tempIntPtr == testValues[1], "Linked list node value did not change despite setting pointer! Current: %d, Expected: %d!", *tempIntPtr, testValues[1]);

	milk_linked_list_free(list);

	list = milk_linked_list_new(sizeof *testValues);
	const int listLength = sizeof testValues / sizeof *testValues;

	for (uint i = 0; i < listLength; i++)
	{
		milk_linked_list_add_first(list, &testValues[i]);
	}

	tempNode = milk_linked_list_iterate(list);

	{
		int firstWrong = -1;
		bool anyWrong = false;
		
		for (int i = listLength - 1; i >= 0; i--)
		{
			FLOOFY_TEST_ASSERT_ARG(tempNode != null, "Ran out of nodes at index %d!", i);
			
			tempInt = *(int*)milk_linked_list_node_value(tempNode);
			
			if (!anyWrong && tempInt != testValues[i])
			{
				firstWrong = i;
				anyWrong = true;
			}

			tempNode = milk_linked_list_node_next(tempNode);
		}
		
		FLOOFY_TEST_ASSERT_ARG(!anyWrong, "Reading linked list in reverse adding order doesn't yield equal values at index %d!", firstWrong);
		
	}

	FLOOFY_TEST_ASSERT(tempNode == null, "Too many nodes in list!");

	int* arr = (int*) milk_linked_list_to_array(list);

	{
		int firstWrong = -1;
		bool anyWrong = false;

		for (uint i = 0; i < listLength; i++)
		{
			if (!anyWrong && arr[i] != testValues[listLength - 1 - i])
			{
				anyWrong = true;
				firstWrong = i;
			}
		}

		FLOOFY_TEST_ASSERT_ARG(!anyWrong, "Values in array aren't equal! Array: %d, Original: %d!", arr[firstWrong], testValues[firstWrong]);
	}
	

	free(arr);
	milk_linked_list_free(list);
}