#include <arguments.h>

#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <util/log.h>
#include <grammar/ast.h>
#include <grammar/parser.h>
#include <codegen/codegen.h>
#include <util/collections/list.h>

const char* argp_program_bug_address = "<typ3def@gmail.com>";
const char* argp_program_version = "x-lang v0.0.1";
static const char* args_doc = "<file.x> ...";
static const char* doc = "x-lang -- Reference compiler for x-lang.\n";

static int parse_opt(int key, char* arg, struct argp_state* state)
{
	struct arguments* a = state->input;
	
	switch (key) {
		case 'o':
			a->outfile = arg;
			break;
			
		case 'i':
			a->ir = true;
			break;
			
		case 'b':
			a->bitcode = arg;
			break;
			
		case 'a':
			a->ast = true;
			break;	
			
		case 'S':
			a->_asm = arg;
			break;
			
		case ARGP_KEY_ARG:
			a->args[state->arg_num] = arg;
			break;
			
			/*case ARGP_KEY_INIT:
			 *		a->args[state->arg_num] = arg;
			 *		break;*/
			
			case ARGP_KEY_END: 
				break;
				
			case 666:
				printf("arg");
				break;
				
	}
	
	return 0;
}

struct arguments parse_arguments(int argc, char** argv)
{
	struct argp_option options[] = {
		{ "output", 'o', "OUTFILE", 0, "Output binary to file.", 0 }, // FIXME
		{ "ir", 'i', 0, 0, "Dump intermediate representation.", 0 },
		{ "ast", 'a', 0, 0, "Dump abstract syntax tree.", 0 },
		{ "asm", 'S', "OUTFILE", 0, "Dump the native assembly code.", 0 },
		{ "bitcode", 'b', "OUTFILE", 0, "Dump the LLVM bitcode to a file.", 0 },
		{ 0 }
	};
	
	struct arguments arguments = { { 0, 0 }, false, false, NULL, NULL, NULL };
	struct argp argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };
	
	if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0)
		return arguments;
    else
        log_kill("Something went horribly wrong parsing arguments, I'm an idiot.\n");
}
