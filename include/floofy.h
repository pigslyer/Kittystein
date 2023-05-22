/*
* Floofy is a unit testing module which depends upon Watchman.
* Functions can be registered to Floofy via the FLOOFY_TEST_REGISTER function, whereupon 
* (assuming __attribute__((constructor)) is supported by the compiler), they will be added on startup to an internal list
* of functions which can be iterated through via floofy_test_run(). 
*
* Tests should use floofy_test_assert in order to test whether their conditions are true. If they are not, their formatted error 
* message will be outputted via a call to watchman_log_error().
* 
* Use #include FLOOFY_EXPOSE_TEST_SUITE to expose internal #defines and functions.
* */

#ifndef FLOOFY_H
#define FLOOFY_H

#include <slyertype.h>

#ifndef FLOOFY_INTERNAL

typedef void (*FloofyTestFunction)(void);

#endif

/// @brief Runs all registered tests, outputting the number of tests run and any failed assertions found along the way.
void floofy_test_run(void);


#ifdef FLOOFY_EXPOSE_TEST_SUITE

/// @brief Registers function to Floofy as runnable test.
/// @param func The new test.
void floofy_test_function(FloofyTestFunction func);

/// @brief Registers function to Floofy as runnable test. This variant is automatically called by FLOOFY_TEST_REGISTER.
/// @param func The new test.
/// @param file The file in which the test is defined.
/// @param line The line on which the test is started.
void floofy_test_function_full(FloofyTestFunction func, const char* file, const uint line, const char* funcName);

/// @brief Checks whether condition is false, printing the formatted message to Floofy's output.
/// @param condition The condition we're checking against.
/// @param error The formatted message to be outputted if the condition is FALSE.
void floofy_test_assert(bool condition, const char* const error, ...);

/// @brief Checks whether condition is false, printing the formatted message to Floofy's output. This function is called automatically
/// by FLOOFY_TEST_ASSERT.
/// @param file The file this assertion failed in.
/// @param line The line on which this assertion failed.
/// @param condition The condition we're checking against.
/// @param error The formatted message to be outputted if the condition is FALSE.
void floofy_test_assert_full(const char* const file, const uint line, bool value, const char* const error, ...);


#define FLOOFY_TEST_REGISTER(test_func_name)																						\
	static void test_func_name(void);																								\
	static void test_func_name##_register(void) __attribute__((constructor));														\
	void test_func_name##_register(void) { floofy_test_function_full(test_func_name, __FILE__, __LINE__, #test_func_name); }

#define FLOOFY_TEST_ASSERT(condition, error)									\
	floofy_test_assert_full(__FILE__, __LINE__, condition, error)

#define FLOOFY_TEST_ASSERT_ARG(condition, error, ...)							\
	floofy_test_assert_full(__FILE__, __LINE__, condition, error, __VA_ARGS__)

#endif

#endif