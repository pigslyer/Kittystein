#include "w_internal.h"

#include <stdio.h>


char* witness_input_get_line_base()
{
	const size_t BUFFER_SIZE = 1024;
	
	char* ret = malloc(0);
	size_t curOffset = 0;

	int cur;

	do
	{
		if (curOffset % BUFFER_SIZE == 0)
		{
			ret = realloc(ret, curOffset + BUFFER_SIZE);
		}

		cur = fgetc(stdin);

		ret[curOffset++] = cur == '\n' ? '\0' : (char)cur;
	} while (cur != '\n');

	return ret;
}

static witness_get_line getLineFunction = witness_input_get_line_base;

char* witness_input_get_line()
{
	return getLineFunction();
}

void witness_input_get_line_set(witness_get_line newGetLineFunction)
{
	getLineFunction = newGetLineFunction;
}

void witness_input_get_line_reset()
{
	getLineFunction = witness_input_get_line_base;
}

// should add support for spaced arguments given wrapping " "
uint witness_input_get_command(char*** params)
{
	char* line = witness_input_get_line();

	uint splitCount;
	*params = delight_string_split(line, ' ', &splitCount);
	
	free(line);

	return splitCount;
}