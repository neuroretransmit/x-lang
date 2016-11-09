#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <grammar/ast.h>
#include <grammar/parser.h>
#include <codegen/codegen.h>
#include <util/log.h>
#include <util/mem_utils.h>
#include <util/file_utils.h>
#include <util/collections/list.h>

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

const char* argp_program_bug_address = "<typ3def@gmail.com>";
const char* argp_program_version = "x-lang v0.0.1";
static const char* args_doc = "<file.x> ...";
static const char* doc = "x-lang -- Reference compiler for x-lang.\n";

struct arguments {
	char* args[2]; /* ARG1 and ARG2 */
	char* ir;
	int ast;
	char* bitcode;
	char* outfile;
};
/*
static void dump_binary(char* fname)
{
	char* llc = "llc -o tmp/tmp.s tmp/tmp.bc";
	char* cc = "cc -o ";
	strcat(cc, fname);
	strcat(cc, " tmp/tmp.s");
	system(llc);
	system(cc);
}

static void dump_bitcode(LLVMModuleRef mod, char* name)
{
	if (LLVMWriteBitcodeToFile(mod, name) != 0) {
		log_err("error writing bitcode to file, skipping\n");
	}
}*/

static void dump_ast(char* fname)
{
	ParserContext* parser = init_parser(fname);
	ASTNode* ast = parse(parser);
	ast_dump(ast);
	destroy_parser(parser);
	destroy_ast_node(ast);
}

static void dump_ir(char* fname)
{
	ParserContext* parser = init_parser(fname);
	ASTNode* ast = parse(parser);
	CodegenContext* context = init_codegen();
	
	codegen(context, ast);
	LLVMBuildRetVoid(context->builder);
	
	char *error = NULL;
	LLVMVerifyModule(context->module, LLVMAbortProcessAction, &error);
	LLVMDisposeMessage(error);
	
	LLVMDumpModule(context->module);

	LLVMRunFunctionAsMain(context->engine, context->main_func, 0, NULL, NULL);
	//dump_binary(context->module, "bin/x-lang.bc");
	
	destroy_codegen(context);
	destroy_parser(parser);
	destroy_ast_node(ast);
}

static int parse_opt(int key, char* arg, struct argp_state* state)
{
	struct arguments* a = state->input;

	switch (key) {
		case 'o':
			a->outfile = arg;
			break;
		
		case 667:
			a->ir = arg;
			break;
		
		case 670:
			a->bitcode = arg;
			break;
		
		/*case 669:
			a->asm = arg;
			break;*/
		
		case 'a':
			a->ast = true;
			break;
			
		case ARGP_KEY_ARG:
			a->args[state->arg_num] = arg;
			break;

		/*case ARGP_KEY_INIT:
			a->args[state->arg_num] = arg;
			break;*/

		case ARGP_KEY_END: 
			break;
		
		case 666:
			printf("arg");
			break;
		
	}

	return 0;
}

int main(int argc, char** argv)
{
	struct argp_option options[] = {
		{ "output", 'o', "OUTFILE", 0, "Output binary to file.", 0 }, // FIXME
		{ "ir", 'i', 0, 0, "Dump intermediate representation.", 0 },
		{ "ast", 'a', 0, 0, "Dump abstract syntax tree.", 0 },
		{ "asm", 0, "OUTFILE", 669, "Dump the native assembly code.", 0 },
		{ "bitcode", 0, "OUTFILE", 670, "Dump the LLVM bitcode to a file.", 0 },
		{ 0 }
	};

	struct arguments arguments;
	struct argp argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

	if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0) {
		//const char* prev = NULL;
		//char* fname;
			if (file_exists(arguments.args[0])) {
				if (arguments.ast)
					dump_ast(arguments.args[0]);
				
				if (arguments.ir) {
					if (arguments.ast)
						puts("===============================");
					dump_ir(arguments.args[0]);
				}
				
				//dump_binary(fname);
			}
			//prev = fname;
		}

	}
