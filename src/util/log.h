#pragma once

#include <stdio.h>
#include <stdlib.h>

#define ANSI_COLOR_RED     "\e[31m"
#define ANSI_COLOR_YELLOW  "\e[33m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN	   "\e[36m"
#define ANSI_COLOR_WHITE   "\e[37m"
#define ANSI_COLOR_RESET   "\e[0m"

#define debug(fmt, ...) \
	do { \
		if (DEBUG) { \
			fprintf(stderr, ANSI_COLOR_CYAN "[DEBUG] " ANSI_COLOR_RESET \
					"%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		} \
	} while (0)

#define log_info(fmt, ...) \
	do { \
		fprintf(stdout, ANSI_COLOR_MAGENTA "[INFO] " ANSI_COLOR_RESET \
				"%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

#define log_warn(fmt, ...) \
	do { \
		fprintf(stderr, ANSI_COLOR_YELLOW "[WARN] " ANSI_COLOR_RESET \
				"%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

#define log_err(fmt, ...) \
	do { \
		fprintf(stderr, ANSI_COLOR_RED "[ERROR] " ANSI_COLOR_RESET \
				"%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

#define log_kill(fmt, ...) \
	do { \
		fprintf(stderr, ANSI_COLOR_RED "[KILLED] " ANSI_COLOR_RESET \
				"%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		exit(1); \
	} while (0)
