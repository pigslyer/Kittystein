#include "w_internal.h"

#include <stdarg.h>
#include <stdio.h>

#include <sys/time.h>
#include <time.h>

typedef enum
{
	W_L_ERROR,
	W_L_WARNING,
	W_L_MESSAGE,
	W_L_COUNT,
} W_LOG_TYPE;

void watchman_log(W_LOG_TYPE type, const char* message, va_list args);

void watchman_log_error(const char* message, ...)
{
	va_list args;
	va_start(args, message);

	watchman_log(W_L_ERROR, message, args);

	va_end(args);
}

void watchman_log_warning(const char* message, ...)
{
	va_list args;
	va_start(args, message);

	watchman_log(W_L_WARNING, message, args);

	va_end(args);
}

void watchman_log_message(const char* message, ...)
{
	va_list args;
	va_start(args, message);

	watchman_log(W_L_MESSAGE, message, args);

	va_end(args);
}

void watchman_log_errorv(const char* message, va_list list)
{
	watchman_log(W_L_ERROR, message, list);
}

void watchman_log_warningv(const char* message, va_list list)
{
	watchman_log(W_L_WARNING, message, list);
}

void watchman_log_messagev(const char* message, va_list list)
{
	watchman_log(W_L_MESSAGE, message, list);
}


void watchman_log(W_LOG_TYPE type, const char* message, va_list args)
{
	// casually stolen from https://www.w3schools.blog/c-get-time-in-milliseconds
	struct timeval epochTime; 
	gettimeofday(&epochTime, null);
	long long milliseconds = epochTime.tv_sec*1000LL + epochTime.tv_usec/1000; // calculate milliseconds
	
	long long sec = (long long) epochTime.tv_sec;
	struct tm* info = localtime(&sec);

	char* log_type;
	switch (type)
	{
		case W_L_ERROR:
		log_type = "[!ERROR!]";

		break;
		
		case W_L_WARNING:
		log_type = "[WARNING]";

		break;
		
		case W_L_MESSAGE:
		default:
		log_type = "[MESSAGE]";
	}

	char buffer_header[4096];
	char buffer_message[4096];

	if (sprintf_s(buffer_header, sizeof buffer_header, "%s %02d:%02d:%02d.%lld: %s", log_type, info->tm_hour, info->tm_min, info->tm_sec, milliseconds, message) <= 0)
	{
		watchman_stream_sendline("CHARACTER BUFFER OVERRAN DURING HEADER GENERATION!");
	} 
	else if (vsprintf_s(buffer_message, sizeof buffer_message, buffer_header, args) <= 0)
	{
		watchman_stream_sendline("CHARACTER BUFFER OVERRAN DURING MESSAGE GENERATION!");	
	}
	else
	{
		watchman_stream_sendline(buffer_message);
	}
}