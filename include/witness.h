#ifndef WITNESS_H
#define WITNESS_H

#include <delight.h>

#ifndef WITNESS_INTERNAL

#endif

typedef void(*witness_function)(uint argc, char** argv);
typedef char*(*witness_get_line)(void);

void witness_loop_start(void);

void witness_loop_end(void);

void witness_input_get_line_set(witness_get_line function);
void witness_input_get_line_reset(void);

//uint witness_autocomplete_options(const char* const from, char** options);

void _witness_register_function(witness_function function, const char* const functionName);

#define WITNESS_REGISTER_FUNCTION(registered_function, command_name)											\
	static void _##registered_function##_register(void) __attribute__((constructor));							\
	void _##registered_function##_register(void) { _witness_register_function(registered_function, command_name)}

#define WITNESS_REGISTER_FUNCTION_NEW(registered_function, command_name)										\
	static void registered_function(...);																		\
	WITNESS_REGISTER_FUNCTION(registered_function, command_name)


#endif