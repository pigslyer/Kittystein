/*
* A logging module, which allows printing formatted errors, warnings and messages.
*
* By default outputs to stdout, this can be overriden via a call to watchman_stream_set_out().
* 
* In the case that, instead of variadic arguments, a va_list parameter is required, use #include WATCHMAN_EXPOSE_VA_LIST
* to expose log_v variants of the 3 log methods.
* 
* Macros also automatically insert line number and file at the start of the message.
* */

#ifndef WATCHMAN_H
#define WATCHMAN_H

#ifndef WATCHMAN_INTERNAL



#endif

/// @brief Logs error to watchman.
/// @param message The intended error message.
void watchman_log_error(const char* message, ...);

/// @brief Logs warning to watchman.
/// @param message The intended warning message.
void watchman_log_warning(const char* message, ...);

/// @brief Logs message to watchman.
/// @param message The intended message.
void watchman_log_message(const char* message, ...);

#define WATCHMAN_LOG_ERROR_ARG(message, ...) 	watchman_log_error("{"__FILE__":%d} "message, __LINE__, __VA_ARGS__)
#define WATCHMAN_LOG_ERROR(message) 			watchman_log_error("{"__FILE__":%d} "message, __LINE__)

#define WATCHMAN_LOG_WARNING_ARG(message, ...) 	watchman_log_warning("{"__FILE__":%d} "message, __LINE__, __VA_ARGS__)
#define WATCHMAN_LOG_WARNING(message) 			watchman_log_warning("{"__FILE__":%d} "message, __LINE__)

#define WATCHMAN_LOG_MESSAGE_ARG(message, ...) 	watchman_log_message("{"__FILE__":%d} "message, __LINE__, __VA_ARGS__)
#define WATCHMAN_LOG_MESSAGE(message) 			watchman_log_message("{"__FILE__":%d} "message, __LINE__)

#ifdef WATCHMAN_EXPOSE_VA_LIST

#include <stdarg.h>

/// @brief Logs error to watchman.
/// @param message The intended error message.
void watchman_log_errorv(const char* message, va_list list);

/// @brief Logs warning to watchman.
/// @param message The intended warning message.
void watchman_log_warningv(const char* message, va_list list);

/// @brief Logs message to watchman.
/// @param message The intended message.
void watchman_log_messagev(const char* message, va_list list);

#endif

#endif