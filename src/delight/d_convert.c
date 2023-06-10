#include "d_internal.h"
#include <watchman.h>

char* delight_convert_int_to_string(int val)
{
	char* ret = malloc(12 * sizeof(char));
	uint idx = 0;

	if (val < 0)
	{
		ret[idx++] = '-';
		val = -val;
	}

	char buffer[12];
	uint i = 0;

	while (val > 0)
	{
		buffer[i++] = (char)('0' + val % 10);
		val /= 10;
	}

	for (uint j = 0; j < i; j++)
	{
		ret[idx++] = buffer[i - j - 1];
	}

	ret[idx] = '\0';

	return ret;
}

char* delight_convert_bool_to_string(bool val)
{
	return delight_string_copy(val ? "true" : "false");
}

int delight_convert_string_to_int(char* str)
{

	int ret = 0;

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

bool delight_convert_string_to_bool(char* str)
{
	return !delight_string_equals(str, "false");
}
