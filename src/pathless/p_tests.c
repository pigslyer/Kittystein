#include "p_internal.h"

#define FLOOFY_EXPOSE_TEST_SUITE
#include <floofy.h>

FLOOFY_TEST_REGISTER(directory_test, true)

void directory_test()
{
	Directory* dir;

	dir = pathless_directory_open("./nonexistent");

	FLOOFY_TEST_ASSERT(dir == null, "Opening a nonexistent directory does not return null!");

	const char* const testRoot = "./src/pathless/p_tests_root";
	dir = pathless_directory_open(testRoot);

	FLOOFY_TEST_ASSERT(dir != null, "Could not open working directory!");
	FLOOFY_TEST_ASSERT_ARG(delight_string_equals(dir->pathToDir, testRoot), "Path to dir is not correct! %s instead of %s!", dir->pathToDir, testRoot);
	FLOOFY_TEST_ASSERT(dir->pathToDir != testRoot, "Dir did not copy string value to its path!");

	uint count = dir->containsCount;

	const char* const expectedContents[] = {
		"file1", "folder1", "folder2"
	};

	const uint expectedLength = sizeof(expectedContents) / sizeof(*expectedContents);

	FLOOFY_TEST_ASSERT_ARG(count == expectedLength, "Dir claims %s does not contain %d files, but instead %d!", testRoot, expectedLength, count);

	for (uint i = 0; i < expectedLength; i++)
	{
		bool hasFound = false;

		for (uint j = 0; j < dir->containsCount; j++)
		{
			if (delight_string_equals(expectedContents[i], dir->contains[j]))
			{
				hasFound = true;
				break;
			}
		}

		FLOOFY_TEST_ASSERT_ARG(hasFound, "Unable to find %s in p_tests root!", expectedContents[i]);
	}

	pathless_directory_close(dir);
}