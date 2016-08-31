#pragma once

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN	   "\x1B[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void log_err(const char* fmt, ...);
void log_warn(const char* fmt, ...);
void log_info(const char* fmt, ...);
void log_kill(const char* fmt, ...);

