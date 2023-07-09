#include "w_internal.h"

#include <watchman.h>

WITNESS_COMMAND_REGISTER_NEW(witness_command_exit, exit)

void witness_command_exit(uint argc, char** argv)
{
	watchman_log_message("Exiting!");
	exit(0);
}

WITNESS_COMMAND_REGISTER_NEW(witness_command_help, help)

void witness_command_help(uint argc, char** argv)
{
	watchman_log_message("Requested help:");

	for (uint i = 0; i < functionCount; i++)
	{
		watchman_log_message(functions[i].functionName);
	}
}