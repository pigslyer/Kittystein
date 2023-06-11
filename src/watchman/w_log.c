#include "w_internal.h"

#include <stdarg.h>
#include <stdio.h>

#include <sys/time.h>
#include <time.h>


void watchman_log(MessageType type, const char* message, va_list args);

void watchman_log_error(const char* message, ...)
{
	va_list args;
	va_start(args, message);

	watchman_log(MESSAGE_ERROR, message, args);

	va_end(args);
}

void watchman_log_warning(const char* message, ...)
{
	va_list args;
	va_start(args, message);

	watchman_log(MESSAGE_WARNING, message, args);

	va_end(args);
}

void watchman_log_message(const char* message, ...)
{
	va_list args;
	va_start(args, message);

	watchman_log(MESSAGE_NORMAL, message, args);

	va_end(args);
}

void watchman_log_errorv(const char* message, va_list list)
{
	watchman_log(MESSAGE_ERROR, message, list);
}

void watchman_log_warningv(const char* message, va_list list)
{
	watchman_log(MESSAGE_WARNING, message, list);
}

void watchman_log_messagev(const char* message, va_list list)
{
	watchman_log(MESSAGE_NORMAL, message, list);
}

#define ANSI_COLOR_RED		"\x1b[31m"
#define ANSI_COLOR_GREEN	"\x1b[32m"
#define ANSI_COLOR_YELLOW	"\x1b[33m"
#define ANSI_COLOR_BLUE		"\x1b[34m"
#define ANSI_COLOR_MAGENTA	"\x1b[35m"
#define ANSI_COLOR_CYAN		"\x1b[36m"
#define ANSI_COLOR_RESET	"\x1b[0m"


void watchman_log(MessageType type, const char* message, va_list args)
{
	// casually stolen from https://www.w3schools.blog/c-get-time-in-milliseconds
	struct timeval epochTime; 
	gettimeofday(&epochTime, null);
	long long microseconds = epochTime.tv_usec;
	
	long long sec = (long long) epochTime.tv_sec;
	struct tm* info = localtime(&sec);

	char* log_type;
	char* log_color;
	switch (type)
	{
		case MESSAGE_ERROR:
		log_type = "[!ERROR!]";
		log_color = ANSI_COLOR_RED;

		break;
		
		case MESSAGE_WARNING:
		log_type = "[WARNING]";
		log_color = ANSI_COLOR_YELLOW;

		break;
		
		case MESSAGE_NORMAL:
		default:
		log_type = "[MESSAGE]";
		log_color = "";
	}

	char buffer_header[4096];
	char buffer_message[4096];

	if (sprintf_s(buffer_header, sizeof buffer_header, "%s%s %02d:%02d:%02d.%06lld: %s"ANSI_COLOR_RESET"\n", log_color, log_type, info->tm_hour, info->tm_min, info->tm_sec, microseconds, message) <= 0)
	{
		watchman_stream_push("CHARACTER BUFFER OVERRAN DURING HEADER GENERATION!\n");
	} 
	else if (vsprintf_s(buffer_message, sizeof buffer_message, buffer_header, args) <= 0)
	{
		watchman_stream_push("CHARACTER BUFFER OVERRAN DURING MESSAGE GENERATION!\n");	
	}
	else
	{
		watchman_stream_push(buffer_message);
	}
}