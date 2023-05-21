#ifndef FLOOFY_H
#define FLOOFY_H

#include <slyertype.h>

#ifndef FLOOFY_INTERNAL

typedef void (*FloofyTestFunction)(void);

#endif

void floofy_test_run(void);
void floofy_test_function(FloofyTestFunction func);
void floofy_test_assert(bool value, const char* const message, ...);

#define FLOOFY_REGISTER_TEST(name, func)								\
	static void name##_function(void);									\
	void name##_function() func											\
	static void name##_register(void) __attribute__((constructor));		\
	void name##_register(void) { floofy_test_function(name##_function); }


#endif