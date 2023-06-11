#include "w_internal.h"

#include <stdio.h>
#include <watchman.h>

static bool witness_running = false;

void witness_loop_start()
{
	witness_running = true;

	while (witness_running)
	{
		char** lines;
		uint paramCount = witness_input_get_command(&lines);

		
	}
}

void witness_loop_end()
{
	witness_running = false;
}