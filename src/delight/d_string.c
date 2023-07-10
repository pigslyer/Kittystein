#include "d_internal.h"

char* string_copy(const char* const sourceString)
{
	u64 length = string_length(sourceString);
	char* ret = (char*) malloc(length + 1);

	memory_copy(ret, sourceString, length);
	ret[length] = '\0';

	return ret;
}

char* string_concat(const char* const str1, const char* const str2)
{
	u64 len1 = string_length(str1);
	u64 len2 = string_length(str2);

	char* ret = (char*) malloc((len1 + len2 + 1) * sizeof(char));

	memory_copy(ret, str1, len1);
	memory_copy(ret + len1, str2, len2);

	ret[len1 + len2] = '\0';

	return ret;
}

void string_append(char** str1, const char* const str2)
{
	char* actualString = *str1;

	u64 len1 = string_length(actualString);
	u64 len2 = string_length(str2);

	actualString = (char*) realloc(actualString, len1 + len2 + 1);

	memory_copy(actualString + len1, str2, len2);

	actualString[len1 + len2] = '\0';

	*str1 = actualString;
}

u64 string_length(const char* const string)
{
	const char* cur;
	for (cur = string; *cur; cur++)
	{ }

	return cur - string;
}


bool string_equals(const char* const string1, const char* const string2)
{
	if (string1 == string2)
	{
		return true;
	}

	u32 i;
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
	static u64 lookup = 0;

	if (lookup != 0)
	{
		return string + lookup;
	}

	for (; string[lookup + 2] != '\0'; lookup++)
	{
		if (string[lookup + 0] == 's' && string[lookup + 1] == 'r' && string[lookup + 2] == 'c')
		{
			return string + lookup;
		}
	}

	return null;
}

bool string_ends_with(const char* const string, const char* const suffix)
{
	u64 str_len = string_length(string);
	u64 suffix_len = string_length(suffix);

	return memory_equals(string + str_len - suffix_len, suffix, suffix_len);
}


u64 string_index_of_char(const char* const string, char value, u64 from)
{
	u64 i;
	for (i = from; string[i] != '\0'; i++)
	{
		if (string[i] == value)
		{
			return i;
		}
	}

	return i;
}

char* string_substring(const char* const string, u64 from, u64 length)
{
	char* ret = malloc(length + 1);

	memory_copy(ret, string + from, length);

	ret[length] = '\0';
	
	return ret;
}

#include <watchman.h>

char** string_split(const char* const string, char byChar, u32* splitCount)
{
	const u64 CHUNK_SIZE = 16;
	
	u64 length = string_length(string);
	char** ret = malloc(CHUNK_SIZE * sizeof *ret);
	u32 count = 0;

	for (u64 cur = 0, temp; cur < length; )
	{
		temp = string_index_of_char(string, byChar, cur);

		ret[count++] = string_substring(string, cur, (temp - cur < length) ? temp - cur : length);

		if (count % CHUNK_SIZE == 0)
		{
			ret = realloc(ret, count + CHUNK_SIZE);
		}

		cur = temp + 1;
	}

	*splitCount = count;

	return ret;
}

char* string_remove_whitespace(const char* const string, bool removeLeading, bool removeEnding)
{
	const char TAB = '	';
	const char SPACE = ' ';

	u64 starting = 0;

	if (removeLeading)
	{
		for (; string[starting] != '\0' && (string[starting] == SPACE || string[starting] == TAB); starting++)
		{ }
	}

	u64 ending = string_length(string) - 1;

	if (removeEnding)
	{
		for (; string[ending] == SPACE || string[ending] == TAB; ending--)
		{ }
	}

	return string_substring(string, starting, ending - starting + 1);
}