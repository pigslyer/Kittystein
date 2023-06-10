#include "d_internal.h"

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

char* _delight_string_make_relative_to_src(char* string)
{

	if (string[0] == '\0' || string[1] == '\0')
	{
		return null;
	}
	
	// for every binary wholly compiled on one system, the relative path's prefix will be the same, so we can reuse it and save ourselves the linear search for "src"
	static size_t lookup = 0;

	if (lookup != 0)
	{
		return string + lookup;
	}

	for (lookup = 0; string[lookup + 2] != '\0'; lookup++)
	{
		if (string[lookup + 0] == 's' && string[lookup + 1] == 'r' && string[lookup + 2] == 'c')
		{
			return string + lookup;
		}
	}

	return null;
}

bool delight_string_ends_with(const char* const string, const char* const suffix)
{
	size_t str_len = delight_string_length(string);
	size_t suffix_len = delight_string_length(suffix);

	return delight_memory_equals(string + str_len - suffix_len, suffix, suffix_len);
}


size_t delight_string_index_of_char(const char* const string, char value, size_t from)
{
	size_t i;
	for (i = from; string[i] != '\0'; i++)
	{
		if (string[i] == value)
		{
			return i;
		}
	}

	return i;
}

char* delight_string_substring(const char* const string, size_t from, size_t length)
{
	char* ret = malloc(length + 1);

	delight_memory_copy(ret, string + from, length);

	ret[length] = '\0';
	
	return ret;
}

#include <watchman.h>

char** delight_string_split(const char* const string, char byChar, uint* splitCount)
{
	const size_t CHUNK_SIZE = 16;
	
	size_t length = delight_string_length(string);
	char** ret = malloc(CHUNK_SIZE * sizeof *ret);
	uint count = 0;

	for (size_t cur = 0, temp; cur < length; )
	{
		temp = delight_string_index_of_char(string, byChar, cur);

		ret[count++] = delight_string_substring(string, cur, (temp - cur < length) ? temp - cur : length);

		if (count % CHUNK_SIZE == 0)
		{
			ret = realloc(ret, count + CHUNK_SIZE);
		}

		cur = temp + 1;
	}

	*splitCount = count;

	return ret;
}