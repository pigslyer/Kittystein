#include "f_internal.h"

#include <stdio.h>

#define WATCHMAN_EXPOSE_VA_LIST
#include <watchman.h>


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
				watchman_log_message("Floofy test \"%s\" completed successfully! Location: {%s:%d}", curTest->functionName, curTest->filePath, curTest->lineNumber);
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
				watchman_log_warning("Floofy test \"%s\" failed! Location: {%s:%d}", curTest->functionName, curTest->filePath, curTest->lineNumber);
			}
			else
			{
				watchman_log_warning("There were %d failures while running unnamed Floofy test!");
			}
		}

		totalTests += floofyTestCount;
		totalFailures += floofyTestFailures;
	}

	WATCHMAN_LOG_MESSAGE_ARG("All %d registered test functions have been run by Floofy. %d out of %d tests were successful!", floofyFunctionCount, totalTests - totalFailures, totalTests);
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
