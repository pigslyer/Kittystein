#include "w_internal.h"

const uint CHUNK_SIZE = 16;

witness_function_data* functions = null;
uint functionCount = 0;

void _witness_commands_register(witness_function function, const char* const name)
{
	if (functions == null)
	{
		functions = malloc(CHUNK_SIZE);
	}
	else if (functionCount % CHUNK_SIZE == 0)
	{
		functions = realloc(functions, functionCount + CHUNK_SIZE);
	}

	witness_function_data* new = &functions[functionCount++];
	new->function = function;
	new->functionName = name;
}
