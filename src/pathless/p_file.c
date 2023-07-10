#include "p_internal.h"

#include <stdio.h>

char* pathless_file_read_as_text(const char* const path, u64* length)
{
	const u64 CHUNK_SIZE = 1024;
	FILE* file = fopen(path, "r");

	if (!file)
	{
		return null;
	}

	u64 curRead;
	u64 curOffset = 0;
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

void pathless_file_write_as_text(const char* const path, char* buffer, u64 length)
{
	FILE* file = fopen(path, "w");

	if (!file)
	{
		return;
	}

	if (length == 0)
	{
		length = string_length(buffer);
	}

	fwrite(buffer, sizeof(char), length, file);

	fclose(file);
}
