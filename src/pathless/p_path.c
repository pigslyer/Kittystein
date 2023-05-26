#include "p_internal.h"

bool pathless_path_is_dir(const char* const path)
{
	size_t len = delight_string_length(path);

	return path[len - 1] == '.' || path[len - 2] == '.' || path[len - 3] == '.' || path[len - 4] == '.';
}