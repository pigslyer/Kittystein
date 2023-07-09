#ifndef WITNESS_H
#define WITNESS_H

#include <delight.h>

#ifndef WITNESS_INTERNAL

#endif

typedef void(*witness_function)(uint argc, char** argv);
typedef void(*witness_function_parameterless)(void);

typedef char*(*witness_get_line)(void);

void witness_commands_execute(uint argc, char** argv);

void _witness_commands_register(witness_function function, const char* const functionName);

#define WITNESS_COMMAND_REGISTER(registered_function, command_name)												\
	static void _##registered_function##_register(void) __attribute__((constructor));							\
	void _##registered_function##_register(void) { _witness_commands_register(registered_function, #command_name);}

#define WITNESS_COMMAND_REGISTER_NEW(registered_function, command_name)											\
	static void registered_function(uint argc, char** argv);													\
	WITNESS_COMMAND_REGISTER(registered_function, command_name)

#endif