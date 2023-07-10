#include "p_internal.h"

#define FLOOFY_EXPOSE_TEST_SUITE
#include <floofy.h>

#include <watchman.h>

FLOOFY_TEST_REGISTER(pathless_directory_test, true)

void pathless_directory_test()
{
	Directory* dir;

	dir = pathless_directory_open("./nonexistent");

	FLOOFY_TEST_ASSERT(dir == null, "Opening a nonexistent directory does not return null!");

	const char* const testRoot = "./src/pathless/p_tests_root";
	dir = pathless_directory_open(testRoot);

	FLOOFY_TEST_ASSERT(dir != null, "Could not open working directory!");
	FLOOFY_TEST_ASSERT_ARG(string_equals(dir->pathToDir, testRoot), "Path to dir is not correct! %s instead of %s!", dir->pathToDir, testRoot);
	FLOOFY_TEST_ASSERT(dir->pathToDir != testRoot, "Dir did not copy string value to its path!");

	u32 count = dir->containsCount;

	const char* const expectedContents[] = {
		"./src/pathless/p_tests_root/file1.f", 
		"./src/pathless/p_tests_root/folder1", 
		"./src/pathless/p_tests_root/folder2",
	};

	const u32 expectedLength = sizeof(expectedContents) / sizeof(*expectedContents);

	FLOOFY_TEST_ASSERT_ARG(count == expectedLength, "Dir claims %s does not contain %d files, but instead %d!", testRoot, expectedLength, count);

	for (u32 i = 0; i < expectedLength; i++)
	{
		bool hasFound = false;

		for (u32 j = 0; j < dir->containsCount; j++)
		{
			if (string_equals(expectedContents[i], dir->contains[j]))
			{			
				hasFound = true;
				break;
			}
		}

		FLOOFY_TEST_ASSERT_ARG(hasFound, "Unable to find %s in p_tests root!", expectedContents[i]);
	}

	pathless_directory_close(dir);
}

FLOOFY_TEST_REGISTER(floofy_iteration_file_test, true)

void floofy_iteration_file_test()
{
	struct PathSettings
	{
		bool recursive;
		bool includeFolders;
		const char* includedFolders[20];
		char* expectedResults[20];
	};

	struct PathSettings tests[] = 
	{
		// 0 -----------------------------
		// all files, full recursion 
		{
			true, false,
			{
				"",
				null,
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				"./src/pathless/p_tests_root/folder1/folder1file1.f",
				"./src/pathless/p_tests_root/folder1/folder1file2.t",
				"./src/pathless/p_tests_root/folder2/folder2file1.f",
				null
			}
		},

		// 1 -----------------------------
		// all files, no recursion
		{
			false, false,
			{
				"",
				null
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				null,
			},
		},

		// 2 -----------------------------
		// no files, full recursion
		{
			true, false,
			{
				null,
			},
			{
				null,
			},
		},

		// 3 ------------------------------
		// no files, no recursion
		{
			false, false,
			{
				null,
			},
			{
				null,
			},
		},

		// 4 ------------------------------
		// .t files, full recursion
		{
			true, false,
			{
				".t",
				null,
			},
			{
				"./src/pathless/p_tests_root/folder1/folder1file2.t",
				null,	
			},
		},
		
		// 5 ------------------------------
		// .t files, no recursion
		{
			false, false,
			{
				".t",
				null,
			},
			{
				null,
			},
		},

		// 6 ------------------------------
		// .f files, full recursion
		{
			true, false,
			{
				".f",
				null
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				"./src/pathless/p_tests_root/folder1/folder1file1.f",
				"./src/pathless/p_tests_root/folder2/folder2file1.f",
				null,
			}
		},

		// 7 -------------------------------
		// .f files, no recursion
		{
			false, false,
			{
				".f",
				null
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				null,
			}
		},
		
		// 8 --------------------------------
		// all files and folders, full recursion
		{
			true, true,
			{
				"",
				null,
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				"./src/pathless/p_tests_root/folder1",
				"./src/pathless/p_tests_root/folder1/folder1file1.f",
				"./src/pathless/p_tests_root/folder1/folder1file2.t",
				"./src/pathless/p_tests_root/folder2",
				"./src/pathless/p_tests_root/folder2/folder2file1.f",
				null,
			},
		},
		
		// 9 --------------------------------
		// all files and folders, no recursion
		{
			false, true,
			{
				"",
				null,
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				"./src/pathless/p_tests_root/folder1",
				"./src/pathless/p_tests_root/folder2",
				null,
			}
		},

		
		// 10 -------------------------------
		// .f files and all folders, full recursion
		{
			true, true,
			{
				".f",
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				"./src/pathless/p_tests_root/folder1",
				"./src/pathless/p_tests_root/folder1/folder1file1.f",
				"./src/pathless/p_tests_root/folder2",
				"./src/pathless/p_tests_root/folder2/folder2file1.f",
				null,
			}
		},
		// 11 -------------------------------
		// .f files and all folders, no recursion
		{
			false, true,
			{
				".f",
			},
			{
				"./src/pathless/p_tests_root/file1.f",
				"./src/pathless/p_tests_root/folder1",
				"./src/pathless/p_tests_root/folder2",
				null,
			}
		}
	};

	struct PathSettings* cur;
	IterationFile* file;
	u32 inclusionCount;

	for (u32 i = 0; i < sizeof tests / sizeof *tests; i++)
	{
		cur = &tests[i];

		for (inclusionCount = 0; cur->includedFolders[inclusionCount] != null; inclusionCount++)
		{ }

		file = pathless_iterate_begin("./src/pathless/p_tests_root", cur->recursive, cur->includeFolders, cur->includedFolders, inclusionCount);

		FLOOFY_TEST_ASSERT(file, "Could not open iterator!");

		u32 total;
		u32 countTooMany = 0;
		u32 countFound = 0;

		while (pathless_iterate_next(file))
		{
			bool found = false;
			for (u32 j = 0; cur->expectedResults[j] != null; j++)
			{
				if (string_equals(pathless_iterate_get_current(file), cur->expectedResults[j]))
				{
					found = true;
					countFound += 1;
					break;
				}

			}
			
			if (!found)
			{
				countTooMany += 1;
			}
		}

		for (total = 0; cur->expectedResults[total] != null; total++)
		{ }

		FLOOFY_TEST_ASSERT_ARG(total == countFound, "Missed %d/%d files during test %d!", total - countFound, total, i);

		FLOOFY_TEST_ASSERT_ARG(countTooMany == 0, "Found %d too many files during test %d!", countTooMany, i);

		pathless_iterate_end(file);
	}
}

FLOOFY_TEST_REGISTER(floofy_text_file_read_test, true)

void floofy_text_file_read_test()
{
	const char* const testText = "zora zora zorica me budi\nopa cupa celo noc je bilo";
	char* text = pathless_file_read_as_text("./src/pathless/p_tests_root/file1.f", null);

	FLOOFY_TEST_ASSERT_ARG(string_equals(text, testText), "Got \n%s\n instead of \n%s\n when reading text file!", text, testText);

	free(text);
}