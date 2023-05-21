#include "w_internal.h"
#include <stdio.h>

void watchman_stream_sendline(char* buffer)
{
	printf("%s\n", buffer);
}