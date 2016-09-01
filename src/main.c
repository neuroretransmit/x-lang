#include <argp.h>
#include <argz.h>
#include <stdio.h>
#include <stdlib.h>

#include "grammar/lexer.h"
#include "util/file_utils.h"

const char* argp_program_bug_address = "<typ3def@gmail.com>";
const char* argp_program_version = "lang-x v0.0.1";
static const char* args_doc = "<file.x> ...";
static const char* doc = "lang-x -- Reference compiler for x-lang.\n";

struct arguments {
	char* argz;
	size_t argz_len;
};

static int parse_opt(int key, char* arg, struct argp_state* state)
{
	struct arguments* a = state->input;
	
    switch (key) {

		case ARGP_KEY_ARG:
			argz_add(&a->argz, &a->argz_len, arg);
			break;
		case ARGP_KEY_INIT:
			a->argz = 0;
			a->argz_len = 0;
			break;
		case ARGP_KEY_END:
		{
			size_t count = argz_count(a->argz, a->argz_len);
		
			if (count > 2)
				argp_failure(state, 1, 0, "Too many source files.");
			else if (count < 1)
				argp_failure(state, 1, 0, "Need a source file.");
		}
		break;
	}

	return 0;
}

int main(int argc, char** argv)
{
	struct argp_option options[] = {
		{ 0 , 0, 0, 0, 0, 0 }
	};

	struct arguments arguments;
	
	struct argp argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };
    
	if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0) {
		const char* prev = NULL;
		char* fname;
		
		while ((fname = argz_next(arguments.argz, arguments.argz_len, prev))) {
			if (file_exists(fname)) {
				init_lexer(fname);
				lex();
			}
			prev = fname;
		}
		free(arguments.argz);
	}
	destroy_lexer();
}
