#pragma once

#include <stdbool.h>

struct arguments {
	char* args[2]; /* ARG1 and ARG2 */
	bool ir;
	bool ast;
	char* bitcode;
	char* _asm;
	char* outfile;
};

struct arguments parse_arguments(int argc, char** argv);

