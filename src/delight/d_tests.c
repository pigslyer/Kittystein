#include "d_internal.h"

#define FLOOFY_EXPOSE_TEST_SUITE
#include <floofy.h>

FLOOFY_TEST_REGISTER(delight_string_test, true)

void delight_string_test()
{
	char string[] = "hello world!";
	uint length = sizeof(string) / sizeof(*string);

	size_t calcLength = delight_string_length(string);
	FLOOFY_TEST_ASSERT_ARG(length-1 == calcLength, "String length not calculated properly. Length should be %d, is %d!", length-1, calcLength);

	FLOOFY_TEST_ASSERT(delight_string_equals(string, "hello world!"), "String equals does not work correctly!");

	char* copy = delight_string_copy(string);

	FLOOFY_TEST_ASSERT_ARG(delight_string_equals(string, copy), "String was not copied properly! Org: %s, Copy: %s!", string, copy);

	char* concatExpected = "hello world!hello world!";
	char* concat = delight_string_concat(copy, string);

	FLOOFY_TEST_ASSERT_ARG(delight_string_equals(concat, concatExpected), "String was not concated properly! Expected: %s, Got: %s", concatExpected, concat);

	delight_string_append(&copy, string);

	FLOOFY_TEST_ASSERT_ARG(delight_string_equals(copy, concatExpected), "String was not appended properly! Expected: %s, Got: %s", concatExpected, copy);

	free(concat);
	free(copy);
}