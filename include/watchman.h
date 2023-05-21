#ifndef WATCHMAN_H
#define WATCHMAN_H

#include <stdarg.h>

void watchman_log_error(const char* message, ...);
void watchman_log_warning(const char* message, ...);
void watchman_log_message(const char* message, ...);

void watchman_log_errorv(const char* message, va_list list);
void watchman_log_warningv(const char* message, va_list list);
void watchman_log_messagev(const char* message, va_list list);

#endif