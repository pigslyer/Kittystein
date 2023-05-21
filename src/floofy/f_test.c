#include "f_internal.h"

#include <slyertype.h>
#include <watchman.h>

FloofyTestFunction registeredFunctions[100];

uint index = 0;

void floofy_test_function(FloofyTestFunction func)
{
	registeredFunctions[index++] = func;
}

void floofy_test_run()
{
	for (uint i = 0; i < index; i++)
	{
		registeredFunctions[i]();
	}
}

void floofy_test_assert(bool value, const char* const error, ...)
{
	if (value)
	{
		va_list args;
		va_start(args, error);

		watchman_log_errorv(error, args);

		va_end(args);
	}
}