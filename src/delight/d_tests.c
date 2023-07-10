
#include "d_internal.h"

#define FLOOFY_EXPOSE_TEST_SUITE
#include <floofy.h>

FLOOFY_TEST_REGISTER(delight_string_test, true)

void delight_string_test()
{
	char string[] = "hello world!";
	u32 length = sizeof(string) / sizeof(*string);

	u64 calcLength = string_length(string);
	FLOOFY_TEST_ASSERT_ARG(length-1 == calcLength, "String length not calculated properly. Length should be %d, is %d!", length-1, calcLength);

	FLOOFY_TEST_ASSERT(string_equals(string, "hello world!"), "String equals does not work correctly!");

	char* copy = string_copy(string);

	FLOOFY_TEST_ASSERT_ARG(string_equals(string, copy), "String was not copied properly! Org: %s, Copy: %s!", string, copy);

	char* concatExpected = "hello world!hello world!";
	char* concat = string_concat(copy, string);

	FLOOFY_TEST_ASSERT_ARG(string_equals(concat, concatExpected), "String was not concated properly! Expected: %s, Got: %s", concatExpected, concat);

	string_append(&copy, string);

	FLOOFY_TEST_ASSERT_ARG(string_equals(copy, concatExpected), "String was not appended properly! Expected: %s, Got: %s", concatExpected, copy);

	FLOOFY_TEST_ASSERT(memory_equals(copy, concatExpected, 10), "Memory compare of strings has failed!");


	free(concat);
	free(copy);

	const char* const relativePath = "src/delight/d_tests.c";

	FLOOFY_TEST_ASSERT_ARG(string_equals(_delight_string_make_relative_to_src(__FILE__), relativePath), "Relative path (%s) was not calculated properly! Got: %s", relativePath, copy);

	FLOOFY_TEST_ASSERT(string_ends_with(string, string), "String does not end with itself!");

	FLOOFY_TEST_ASSERT(string_ends_with(string, "ld!"), "String ends does not work properly!");
	FLOOFY_TEST_ASSERT(!string_ends_with(string, "old"), "String ends does not work properly!");
	

	FLOOFY_TEST_ASSERT(string_ends_with(string, ""), "String should always end with an empty string!");


	const char* const whiteSpaceString = "  a string with some whitespace 	";
	const char* const noLeadingWhitespaceString = "a string with some whitespace 	";
	const char* const noEndingWhitespaceString = "  a string with some whitespace";
	const char* const noWhitespaceString = "a string with some whitespace";

	char* whitespaceTestString = string_remove_whitespace(whiteSpaceString, false, false);
	FLOOFY_TEST_ASSERT_ARG(string_equals(whitespaceTestString, whiteSpaceString), "Removal using no whitespace returned \"%s\"!", whitespaceTestString);
	free(whitespaceTestString);

	whitespaceTestString = string_remove_whitespace(whiteSpaceString, true, false);
	FLOOFY_TEST_ASSERT_ARG(string_equals(whitespaceTestString, noLeadingWhitespaceString), "Removal using leading whitespace returned \"%s\"!", whitespaceTestString);
	free(whitespaceTestString);
	

	whitespaceTestString = string_remove_whitespace(whiteSpaceString, false, true);
	FLOOFY_TEST_ASSERT_ARG(string_equals(whitespaceTestString, noEndingWhitespaceString), "Removal using ending whitespace returned \"%s\"!", whitespaceTestString);
	free(whitespaceTestString);
	
	whitespaceTestString = string_remove_whitespace(whiteSpaceString, true, true);
	FLOOFY_TEST_ASSERT_ARG(string_equals(whitespaceTestString, noWhitespaceString), "Removal using all whitespace returned \"%s\"!", whitespaceTestString);
	free(whitespaceTestString);

	whitespaceTestString = string_remove_whitespace(noWhitespaceString, true, true);
	FLOOFY_TEST_ASSERT_ARG(string_equals(whitespaceTestString, noWhitespaceString), "Removal of non-existent whitespace returned \"%s\"!", whitespaceTestString);
	free(whitespaceTestString);
}

#ifndef DELIGHT_MEMORY_TRACKER
#define DELIGHT_MEMORY_TRACKER
#endif
#define DELIGHT_MEMORY_DEBUG_TOOLS
#include <delight.h>

FLOOFY_TEST_REGISTER(delight_memory_test, false)

void delight_memory_test()
{
	const u64 allocAmount = 25;

	// we can't be sure when the test runs and with how much memory, all measurements will have to be relative to the starting allocation
	// we should also pause all threads in here, but i don't know how to do threads yet :)
	size_t startingAlloc, tempAlloc;
	void* tempMem;

	memory_usage_report(null, &startingAlloc);

	tempMem = malloc(allocAmount);

	memory_usage_report(null, &tempAlloc);

	FLOOFY_TEST_ASSERT_ARG(tempAlloc - startingAlloc == allocAmount, "Malloced memory was not kept track of! After mallocing %lld, the delta is %lld!", allocAmount, tempAlloc - startingAlloc);

	free(tempMem);

	memory_usage_report(null, &tempAlloc);

	FLOOFY_TEST_ASSERT_ARG(tempAlloc == startingAlloc, "Malloced memory was not freed! After freeing %lld, the delta is %lld!", allocAmount, tempAlloc - startingAlloc);

	{	
		bool anyWrong = false;
		const u64 elementCount = 10;

		int* callocTest = calloc(elementCount, sizeof *callocTest);

		memory_usage_report(null, &tempAlloc);

		FLOOFY_TEST_ASSERT_ARG(tempAlloc - startingAlloc == elementCount * sizeof *callocTest, "Calloced memory was not kept track of! After callocing %lld, the delta is %lld!", elementCount * sizeof * callocTest, tempAlloc - startingAlloc);

		for (u64 i = 0; i < elementCount; i++)
		{
			if (!anyWrong && callocTest[i] != 0)
			{
				anyWrong = true;
			}
		}

		FLOOFY_TEST_ASSERT(!anyWrong, "Calloced memory is not empty!");

		free(callocTest);

		
		memory_usage_report(null, &tempAlloc);

		FLOOFY_TEST_ASSERT_ARG(tempAlloc == startingAlloc, "Calloced memory was not freed! After freeing %lld, the delta is %lld!", allocAmount, tempAlloc - startingAlloc);
	}
	
	{
		i32 testArray[] = {10, 20, 30, 40, 50, 60};
		const u64 reallocSize = 10 * sizeof *testArray;

		// we can't use memory_duplicate here because memory tracking might not be turned on in it
		tempMem = malloc(sizeof testArray);

		memory_copy(tempMem, testArray, sizeof testArray);

		FLOOFY_TEST_ASSERT(memory_equals(tempMem, testArray, sizeof testArray), "Not properly equal?");

		tempMem = realloc(tempMem, reallocSize);

		FLOOFY_TEST_ASSERT(memory_equals(tempMem, testArray, sizeof testArray), "Nope");

		memory_usage_report(null, &tempAlloc);

		FLOOFY_TEST_ASSERT_ARG(tempAlloc - startingAlloc == reallocSize, "Realloced memory was not kept track of! After reallocing %lld, the delta is %lld!", reallocSize, tempAlloc - startingAlloc);

		bool anyWrong = false;

		i32* table = (i32*) tempMem;
		for (size_t i = 0; i < sizeof testArray / sizeof *testArray; i++)
		{
			if (!anyWrong && table[i] != testArray[i])
			{
				anyWrong = true;
			}
		}

		FLOOFY_TEST_ASSERT(!anyWrong, "Memory was not properly realloced!");

		free(tempMem);

		memory_usage_report(null, &tempAlloc);

		FLOOFY_TEST_ASSERT_ARG(tempAlloc == startingAlloc, "Realloced memory was not freed! After freeing %lld, the delta is %lld!", allocAmount, tempAlloc - startingAlloc);	
	}

	{
		u64 startingAllocCount;
		memory_usage_report(&startingAllocCount, null);

		const u64 singleAllocationAmount = 5;
		const u64 allocationCount = 2000;

		void* temp[allocationCount];

		for (u64 i = 0; i < allocationCount; i++)
		{
			temp[i] = malloc(singleAllocationAmount + i);
		}

		u64 readAllocationCount;

		memory_usage_report(&readAllocationCount, &tempAlloc);

		FLOOFY_TEST_ASSERT_ARG(readAllocationCount - startingAllocCount == allocationCount, "Allocation count was not correctly tracked! Currently: %lld, When it's supposed to be: %lld", readAllocationCount - startingAllocCount, allocationCount);

		bool anyWrong = false;

		for (u64 i = 0; i < allocationCount; i++)
		{
			if (!anyWrong && memory_usage_allocation_amount(i + startingAllocCount) != singleAllocationAmount + i)
			{
				anyWrong = true;
			}
		}

		FLOOFY_TEST_ASSERT(!anyWrong, "Malloced allocation memory wasn't properly tracked!");

		
		anyWrong = false;

		for (u64 i = 0; i < allocationCount; i++)
		{
			if (!anyWrong && memory_usage_allocation_location(i + startingAllocCount) != temp[i])
			{
				anyWrong = true;
			}
		}

		FLOOFY_TEST_ASSERT(!anyWrong, "Malloced allocation memory wasn't properly tracked!");


		for (u64 i = 0; i < allocationCount; i++)
		{
			free(temp[i]);
		}

		memory_usage_report(&readAllocationCount, null);

		FLOOFY_TEST_ASSERT(readAllocationCount == startingAllocCount, "Not all memory was freed!");
	}
}