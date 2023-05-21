#include "f_internal.h"

#include <stdio.h>

#include <slyertype.h>

#define WATCHMAN_EXPOSE_VA_LIST
#include <watchman.h>

FloofyTestFunction registeredFunctions[100];
uint registeredFunctionCount = 0;

void floofy_test_function(FloofyTestFunction func)
{
	registeredFunctions[registeredFunctionCount++] = func;
}

void floofy_test_function_full(FloofyTestFunction func, char* file, uint line, char* funcName)
{
	registeredFunctions[registeredFunctionCount++] = func;
}

void floofy_test_run()
{
	for (uint i = 0; i < registeredFunctionCount; i++)
	{
		registeredFunctions[i]();
	}

	WATCHMAN_LOG_MESSAGE_ARG("All %d registered test functions have been run by Floofy.", registeredFunctionCount);
}

void floofy_test_assert(bool value, const char* const error, ...)
{
	if (!value)
	{
		va_list args;
		va_start(args, error);

		watchman_log_errorv(error, args);

		va_end(args);
	}
}

void floofy_test_assert_full(char* file, uint line, bool value, const char* const error, ...)
{
	if (!value)
	{
		char buffer[4096];
		
		if (sprintf_s(buffer, sizeof buffer, "{%s:%d} - %s", file, line, error) > 0)
		{
			va_list args;
			va_start(args, error);

			watchman_log_errorv(buffer, args);

			va_end(args);
		}
		else
		{
			watchman_log_error("Unit test error ocurred in %s on line %d, message overflowed.");
		}
	}
}
