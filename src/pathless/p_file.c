#include "p_internal.h"

#include <watchman.h>

#include <stdio.h>

char* pathless_file_read_as_text(const char* const path, size_t* length)
{
	const size_t CHUNK_SIZE = 1024;
	FILE* file = fopen(path, "r");

	if (!file)
	{
		return null;
	}

	size_t curRead;
	size_t curOffset = 0;
	char* ret = malloc(CHUNK_SIZE);

	while ((curRead = fread(ret + curOffset, sizeof(char), CHUNK_SIZE, file)) == CHUNK_SIZE)
	{
		curOffset += CHUNK_SIZE;
		ret = realloc(ret, CHUNK_SIZE + curOffset);
	}

	ret[curOffset + curRead] = '\0';
	
	if (length)
	{
		*length = curOffset + curRead;
	}

	return ret;
}

void pathless_file_write_as_text(const char* const path, char* buffer, size_t length)
{
	FILE* file = fopen(path, "w");

	if (!file)
	{
		return;
	}

	if (length == 0)
	{
		length = delight_string_length(buffer);
	}

	fwrite(buffer, sizeof(char), length, file);

	fclose(file);
}