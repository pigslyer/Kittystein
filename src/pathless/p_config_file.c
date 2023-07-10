#include "p_internal.h"

static const u32 CATEGORY_CHUNK_SIZE = 16;
static const u32 KEY_CHUNK_SIZE = 16;

ConfigFile* pathless_config_file_new(void)
{
	ConfigFile* ret = malloc(sizeof *ret);
	ret->categories = malloc(CATEGORY_CHUNK_SIZE * sizeof *ret->categories);
	ret->categoryLength = CATEGORY_CHUNK_SIZE;
	ret->categoryCount = 0;

	return ret;
}

void pathless_config_file_free(ConfigFile* file)
{
	pathless_config_file_clear(file);
	free(file->categories);
	free(file);
}

void pathless_config_file_clear(ConfigFile* file)
{
	ConfigCategory* curCategory;
	for (u32 i = 0; i < file->categoryCount; i++)
	{
		curCategory = &file->categories[i];
		for (u32 j = 0; j < curCategory->pairCount; j++)
		{
			free(curCategory->keys[j]);
			free(curCategory->values[j]);
		}

		free(curCategory->keys);
		free(curCategory->values);
		free(curCategory->categoryName);
		free(curCategory);
	}

	file->categoryCount = 0;
}

void pathless_config_file_save(ConfigFile* file, const char* const path)
{
	const u64 BUFFER_CHUNK_SIZE = 1024;
	char* buffer = malloc(BUFFER_CHUNK_SIZE);
	char* movedBuffer = buffer;

	ConfigCategory* curCategory;
	u64 stringLength;
	for (u32 i = 0; i < file->categoryCount; i++)
	{
		curCategory = &file->categories[i];
		stringLength = string_length(curCategory->categoryName);
		
		movedBuffer[0] = '[';
		movedBuffer += 1;

		memory_copy(movedBuffer, curCategory->categoryName, stringLength);
		movedBuffer += stringLength;

		memory_copy(movedBuffer, "]\n\n", 3);
		movedBuffer += 3;

		for (u32 j = 0; j < curCategory->pairCount; j++)
		{
			stringLength = string_length(curCategory->keys[j]);
			
			memory_copy(movedBuffer, curCategory->keys[j], stringLength);
			movedBuffer += stringLength;

			movedBuffer[0] = '=';
			movedBuffer += 1;

			stringLength = string_length(curCategory->values[j]);

			memory_copy(movedBuffer, curCategory->values[j], stringLength);
			movedBuffer += stringLength;

			movedBuffer[0] = '\n';
			movedBuffer += 1;
		}

		movedBuffer[0] = '\n';
		movedBuffer += 1;
	}

	pathless_file_write_as_text(path, buffer, movedBuffer - buffer);
	free(buffer);
}

void pathless_config_file_load(ConfigFile* file, const char* const path)
{
	pathless_config_file_clear(file);

	u64 fileLength;
	char* buffer = pathless_file_read_as_text(path, &fileLength);

	u32 newLineCount;
	char** newLineSplit = string_split(buffer, '\n', &newLineCount);

	char* curCategory = null;

	for (u32 i = 0; i < newLineCount; i++)
	{
		u32 lineSplitCount;
		char** lineSplit = string_split(newLineSplit[i], '=', &lineSplitCount);

		if (lineSplitCount == 1)
		{
			if (curCategory)
			{
				free(curCategory);
			}

			curCategory = string_substring(lineSplit[0], 1, string_length(lineSplit[0]) - 2);
			
			free(lineSplit[0]);
		}
		else if (lineSplitCount > 1)
		{
			u64 equalsPos = string_index_of_char(newLineSplit[i], '=', 0);
			char* savedValue = string_substring(newLineSplit[i], equalsPos + 1, string_length(newLineSplit[i]) - equalsPos);
			
			pathless_config_file_save_string(
				file, 
				curCategory, 
				lineSplit[0], 
				savedValue	
			);
			
			free(savedValue);

			for (u32 j = 0; j < lineSplitCount; j++)
			{
				free(lineSplit[j]);
			}
		}

		free(lineSplit);
		free(newLineSplit[i]);
	}

	if (curCategory)
	{
		free(curCategory);
	}

	free(newLineSplit);
	free(buffer);
}

ConfigCategory* pathless_config_file_category_append(ConfigFile* file, const char* const category);
ConfigCategory* pathless_config_file_category_get(ConfigFile* file, const char* const category);
int pathless_config_file_category_get_key(ConfigCategory* category, const char* const key, bool createIfNonExistent);
void pathless_config_file_category_delete_key(ConfigCategory* category, const char* const key);

ConfigCategory* pathless_config_file_category_append(ConfigFile* file, const char* const category)
{
	if (file->categoryCount == file->categoryLength)
	{
		file->categories = realloc(file->categories, file->categoryLength + CATEGORY_CHUNK_SIZE);
		file->categoryLength += CATEGORY_CHUNK_SIZE;
	}

	ConfigCategory* ret = &file->categories[file->categoryCount++];
	ret->categoryName = string_copy(category);
	ret->keys = malloc(KEY_CHUNK_SIZE * sizeof *ret->keys);
	ret->values = malloc(KEY_CHUNK_SIZE * sizeof *ret->values);
	ret->pairCount = 0;
	ret->pairLength = KEY_CHUNK_SIZE;

	return ret;
}

ConfigCategory* pathless_config_file_category_get(ConfigFile* file, const char* const category)
{
	for (u32 i = 0; i < file->categoryCount; i++)
	{
		if (string_equals(file->categories[i].categoryName, category))
		{
			return &file->categories[i];
		}
	}

	return null;
}

void pathless_config_file_remove_category(ConfigFile* file, const char* const category)
{
	ConfigCategory* deleting = pathless_config_file_category_get(file, category);

	if (deleting == null)
	{
		return;
	}

	const u32 count = deleting->pairCount;

	for (u32 i = 0; i < count; i++)
	{
		free(deleting->keys[i]);
		free(deleting->values[i]);
	}

	free(deleting->keys);
	free(deleting->values);
	free(deleting->categoryName);
	free(deleting);
}

void pathless_config_file_remove_key(ConfigFile* file, const char* const category, const char* const key)
{
	ConfigCategory* cat = pathless_config_file_category_get(file, category);

	if (cat == null)
	{
		return;
	}

	pathless_config_file_category_delete_key(cat, key);
}


void pathless_config_file_category_delete_key(ConfigCategory* category, const char* const key)
{
	int keyIndex = pathless_config_file_category_get_key(category, key, false);

	if (keyIndex == -1)
	{
		return;
	}

	free(category->keys[keyIndex]);
	free(category->values[keyIndex]);

	category->pairCount -= 1;

	category->keys[keyIndex] = category->keys[category->pairCount];
	category->values[keyIndex] = category->values[category->pairCount];
}

int pathless_config_file_category_get_key(ConfigCategory* category, const char* const key, bool createIfNonExistent)
{
	for (u32 i = 0; i < category->pairCount; i++)
	{
		if (string_equals(category->keys[i], key))
		{
			return i;
		}
	}

	if (!createIfNonExistent)
	{
		return -1;
	}

	if (category->pairLength == category->pairCount)
	{
		category->pairLength += KEY_CHUNK_SIZE;
		category->keys = realloc(category->keys, category->pairLength);
		category->values = realloc(category->values, category->pairLength);
	}
	
	category->values[category->pairCount] = null;
	category->keys[category->pairCount] = null;

	return category->pairCount++;
}

void pathless_config_file_save_string(ConfigFile* file, const char* const category, const char* const key, const char* const value)
{
	ConfigCategory* chosen = pathless_config_file_category_get(file, category);

	if (!chosen)
	{
		chosen = pathless_config_file_category_append(file, category);
	}

	int idx = pathless_config_file_category_get_key(chosen, key, true);

	if (chosen->keys[idx])
	{
		free(chosen->keys[idx]);
	}
	
	if (chosen->values[idx])
	{
		free(chosen->values[idx]);
	}

	chosen->keys[idx] = string_copy(key);
	chosen->values[idx] = string_copy(value);	
}

void pathless_config_file_save_int(ConfigFile* file, const char* const category, const char* const key, int value)
{
	ConfigCategory* chosen = pathless_config_file_category_get(file, category);

	if (!chosen)
	{
		chosen = pathless_config_file_category_append(file, category);
	}

	int idx = pathless_config_file_category_get_key(chosen, key, true);
	
	if (chosen->keys[idx])
	{
		free(chosen->keys[idx]);
	}
	
	if (chosen->values[idx])
	{
		free(chosen->values[idx]);
	}

	chosen->keys[idx] = string_copy(key);
	chosen->values[idx] = convert_i32_to_string(value);
}

void pathless_config_file_save_bool(ConfigFile* file, const char* const category, const char* const key, bool value)
{
	ConfigCategory* chosen = pathless_config_file_category_get(file, category);

	if (!chosen)
	{
		chosen = pathless_config_file_category_append(file, category);
	}

	int idx = pathless_config_file_category_get_key(chosen, key, true);

	if (chosen->keys[idx])
	{
		free(chosen->keys[idx]);
	}
	
	if (chosen->values[idx])
	{
		free(chosen->values[idx]);
	}

	chosen->keys[idx] = string_copy(key);
	chosen->values[idx] = convert_bool_to_string(value);
}

bool pathless_config_file_try_load_string(ConfigFile* file, const char* const category, const char* const key, char** value)
{
	ConfigCategory* chosen = pathless_config_file_category_get(file, category);

	if (!chosen)
	{	
		return false;
	}

	int idx = pathless_config_file_category_get_key(chosen, key, false);

	if (idx == -1)
	{
		return false;
	}

	*value = chosen->values[idx];

	return true;
}

bool pathless_config_file_try_load_int(ConfigFile* file, const char* const category, const char* const key, int* value)
{
	ConfigCategory* chosen = pathless_config_file_category_get(file, category);

	if (!chosen)
	{
		return false;
	}

	int idx = pathless_config_file_category_get_key(chosen, key, false);

	if (idx == -1)
	{
		return false;
	}

	*value = convert_string_to_i32(chosen->values[idx]);

	return true;
}

bool pathless_config_file_try_load_bool(ConfigFile* file, const char* const category, const char* const key, bool* value)
{
	ConfigCategory* chosen = pathless_config_file_category_get(file, category);

	if (!chosen)
	{
		return false;
	}

	int idx = pathless_config_file_category_get_key(chosen, key, false);

	if (idx == -1)
	{
		return false;
	}

	*value = convert_string_to_bool(chosen->values[idx]);

	return true;
}

char** pathless_config_file_get_categories(ConfigFile* file, u32* count)
{
	u32 arrLen = *count = file->categoryCount;
	
	char** ret = malloc(arrLen * sizeof *ret);

	for (u32 i = 0; i < arrLen; i++)
	{
		ret[i] = string_copy(file->categories[i].categoryName);
	}

	return ret;
}

char** pathless_config_file_get_keys(ConfigFile* file, const char* const category, u32* count)
{
	ConfigCategory* chosen = pathless_config_file_category_get(file, category);

	if (chosen == null)
	{
		*count = 0;
		return null;
	}

	u32 arrLen = *count = chosen->pairCount;
	char** ret = malloc(arrLen * sizeof *ret);

	for (u32 i = 0; i < arrLen; i++)
	{
		ret[i] = string_copy(chosen->keys[i]);
	}

	return ret;
}
