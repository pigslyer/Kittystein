#define DELIGHT_MEMORY_DEBUG_TOOLS
#include "f_internal.h"

#include <stdio.h>

#define WATCHMAN_EXPOSE_VA_LIST
#include <watchman.h>
#include <witness.h>

FloofyTest floofyRegisteredFunctions[100];
uint floofyFunctionCount = 0;

void floofy_test_function(FloofyTestFunction func)
{
	floofyRegisteredFunctions[floofyFunctionCount++].functionCallback = func;
}

void floofy_test_function_full(FloofyTestFunction func, const char* const file, const uint line, const char* const funcName)
{
	FloofyTest* curTest = &floofyRegisteredFunctions[floofyFunctionCount++];

	curTest->functionCallback = func;
	curTest->functionName = funcName;
	curTest->filePath = file;
	curTest->lineNumber = line;
}

uint floofyTestCount;
uint floofyTestFailures;

void floofy_test_run()
{
	ulong preAllocCount, postAllocCount;
	size_t preAlloc, postAlloc;
	delight_memory_usage_report(&preAllocCount, &preAlloc);

	FloofyTest* curTest;

	floofyTestCount = 0;
	uint totalFailures = 0;
	uint totalTests = 0;

	for (uint i = 0; i < floofyFunctionCount; i++)
	{
		curTest = &floofyRegisteredFunctions[i];

		floofyTestFailures = 0;
		floofyTestCount = 0;

		curTest->functionCallback();

		if (floofyTestFailures == 0)
		{

			if (curTest->functionName != null)
			{
				watchman_log_message("Floofy test \"%s\" completed successfully! Location: {./%s:%d}", curTest->functionName, curTest->filePath, curTest->lineNumber);
			}
			else
			{
				watchman_log_message("Unnamed Floofy test completed successfully!");
			}
		}
		else
		{

			if (curTest->functionName != null)
			{
				watchman_log_warning("Floofy test \"%s\" failed! Location: {./%s:%d}", curTest->functionName, curTest->filePath, curTest->lineNumber);
			}
			else
			{
				watchman_log_warning("There were %d failures while running unnamed Floofy test!");
			}
		}

		totalTests += floofyTestCount;
		totalFailures += floofyTestFailures;
	}

	watchman_log_message("All %d registered test functions have been run by Floofy. %d out of %d tests were successful!", floofyFunctionCount, totalTests - totalFailures, totalTests);

	delight_memory_usage_report(&postAllocCount, &postAlloc);

	if (preAlloc != postAlloc)
	{
		watchman_log_error("There is a memory leak somewhere in the tests! Total leaked memory: %lld bytes", postAlloc - preAlloc);
		watchman_log_message("Performing memory dump:");


		const char* locationFile;
		uint locationLine;
		size_t amount;
		for (ulong i = preAllocCount; i < postAllocCount; i++)
		{
			delight_memory_usage_request_location(i, &locationFile, &locationLine);
			amount = delight_memory_usage_allocation_amount(i);

			watchman_log_message("Allocation % 3lld: Reserved % 6lld bytes at {./%s:%d}.", i - preAllocCount, amount, locationFile, locationLine);
		}
	}
	else
	{
#ifdef DELIGHT_MEMORY_TRACKER
		watchman_log_message("No memory was leaked!");
#else
		watchman_log_message("Memory tracking was not enabled!");
#endif
	}
}

void floofy_test_assert(bool value, const char* const error, ...)
{
	floofyTestCount += 1;

	if (!value)
	{
		floofyTestFailures += 1;

		va_list args;
		va_start(args, error);

		watchman_log_errorv(error, args);

		va_end(args);
	}
}

void floofy_test_assert_full(const char* const file, const uint line, bool value, const char* const error, ...)
{
	floofyTestCount += 1;

	if (!value)
	{
		floofyTestFailures += 1;

		char buffer[4096];
		
		if (sprintf_s(buffer, sizeof buffer, "{./%s:%d} - %s", file, line, error) > 0)
		{
			va_list args;
			va_start(args, error);

			watchman_log_errorv(buffer, args);

			va_end(args);
		}
		else
		{
			watchman_log_error("Unit test error ocurred in ./%s on line %d, message overflowed.");
		}
	}
}
