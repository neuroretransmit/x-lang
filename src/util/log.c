#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void log_err(const char* fmt, ...)
{	
	fprintf(stderr, "[%sERROR%s] ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
	
	va_list args;
	
	va_start(args, fmt);	
	vprintf(fmt, args);
	va_end(args);
}

void log_warn(const char* fmt, ...)
{
	fprintf(stderr, "[%sWARN%s] ", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	
	va_list args;
	
	va_start(args, fmt);	
	vprintf(fmt, args);
	va_end(args);
}

void log_info(const char* fmt, ...)
{
	fprintf(stdout, "[%sINFO%s] ", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
	
	va_list args;
	
	va_start(args, fmt);	
	vprintf(fmt, args);
	va_end(args);
}

void log_kill(const char* fmt, ...)
{	
	fprintf(stderr, "[%sKILLED%s] [%s:%d] ", ANSI_COLOR_RED, ANSI_COLOR_RESET, __FILE__, __LINE__);
	
	va_list args;
	
	va_start(args, fmt);	
	vprintf(fmt, args);
	va_end(args);
	
	exit(1);
}
