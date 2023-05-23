#include "d_internal.h"

#include <string.h>

char* delight_string_copy(const char* const sourceString)
{
	size_t length = delight_string_length(sourceString);
	char* ret = (char*) malloc(length + 1);

	delight_memory_copy(ret, sourceString, length);
	ret[length] = '\0';

	return ret;
}

char* delight_string_concat(const char* const str1, const char* const str2)
{
	size_t len1 = delight_string_length(str1);
	size_t len2 = delight_string_length(str2);

	char* ret = (char*) malloc((len1 + len2 + 1) * sizeof(char));

	delight_memory_copy(ret, str1, len1);
	delight_memory_copy(ret + len1, str2, len2);

	ret[len1 + len2] = '\0';

	return ret;
}

void delight_string_append(char** str1, const char* const str2)
{
	char* actualString = *str1;

	size_t len1 = delight_string_length(actualString);
	size_t len2 = delight_string_length(str2);

	actualString = (char*) realloc(actualString, len1 + len2 + 1);

	delight_memory_copy(actualString + len1, str2, len2);

	actualString[len1 + len2] = '\0';

	*str1 = actualString;
}

size_t delight_string_length(const char* const string)
{
	const char* cur;
	for (cur = string; *cur; cur++)
	{ }

	return cur - string;
}


bool delight_string_equals(const char* const string1, const char* const string2)
{
	if (string1 == string2)
	{
		return true;
	}

	uint i;
	for (i = 0; string1[i] != '\0' && string2[i] != '\0'; i++)
	{
		if (string1[i] != string2[i])
		{
			return false;
		}
	}

	return string1[i] == string2[i];
}