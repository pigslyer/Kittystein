#include "d_internal.h"
#include <watchman.h>

char* convert_i32_to_string(i32 val)
{
	char* ret = malloc(12 * sizeof(char));
	u32 idx = 0;

	if (val < 0)
	{
		ret[idx++] = '-';
		val = -val;
	}

	char buffer[12];
	u32 i = 0;

	while (val > 0)
	{
		buffer[i++] = (char)('0' + val % 10);
		val /= 10;
	}

	for (u32 j = 0; j < i; j++)
	{
		ret[idx++] = buffer[i - j - 1];
	}

	ret[idx] = '\0';

	return ret;
}

char* convert_bool_to_string(bool val)
{
	return string_copy(val ? "true" : "false");
}

i32 convert_string_to_i32(char* str)
{

	i32 ret = 0;

	bool neg = str[0] == '-';

	for (char* c = str + neg; *c; c++)
	{
		ret = ret * 10 + (*c) - '0';
	}

	if (neg)
	{
		return -ret;
	}
	
	return ret;
}

bool convert_string_to_bool(char* str)
{
	return !string_equals(str, "false");
}
