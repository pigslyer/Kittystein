#include "w_internal.h"

#include <watchman.h>

WITNESS_COMMAND_REGISTER_NEW(witness_command_exit, exit)

void witness_command_exit(u32 argc, char** argv)
{
	watchman_log_message("Exiting!");
	exit(0);
}

WITNESS_COMMAND_REGISTER_NEW(witness_command_help, help)

void witness_command_help(u32 argc, char** argv)
{
	watchman_log_message("Requested help:");

	for (u32 i = 0; i < functionCount; i++)
	{
		watchman_log_message(functions[i].functionName);
	}
}