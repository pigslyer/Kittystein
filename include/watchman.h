/*
* A logging module, which allows printing formatted errors, warnings and messages.
*
* By default outputs to stdout, this can be overriden via a call to watchman_stream_set_out().
* 
* In the case that, instead of variadic arguments, a va_list parameter is required, use #include WATCHMAN_EXPOSE_VA_LIST
* to expose log_v variants of the 3 log methods.
* */

#ifndef WATCHMAN_H
#define WATCHMAN_H

#include <stdarg.h>

/// @brief Logs error to watchman.
/// @param message The intended error message.
void watchman_log_error(const char* message, ...);

/// @brief Logs warning to watchman.
/// @param message The intended warning message.
void watchman_log_warning(const char* message, ...);

/// @brief Logs message to watchman.
/// @param message The intended message.
void watchman_log_message(const char* message, ...);


#ifdef WATCHMAN_EXPOSE_VA_LIST

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