#include "m_internal.h"

#include <stdlib.h>
#include <string.h>

void* milk_util_shallow_copy_new(void* what, size_t length)
{
	void* ret = malloc(length);
	memcpy(ret, what, length);

	return ret;
}

void milk_util_shallow_copy_free(void* what)
{
	free(what);
}