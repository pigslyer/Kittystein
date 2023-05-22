#include "w_internal.h"
#include <stdio.h>

void watchman_stream_stdout(char* message);

WatchmanStreamCallback watchmanOut = watchman_stream_stdout;



void watchman_stream_push(char* message)
{
	watchmanOut(message);
}

void watchman_stream_out_set(WatchmanStreamCallback callback)
{
	watchmanOut = callback;
}

void watchman_stream_out_reset()
{
	watchmanOut = watchman_stream_stdout;
}

void watchman_stream_stdout(char* message)
{
	printf("%s", message);
}