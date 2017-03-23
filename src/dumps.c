#include "dumps.h"

#include <stdlib.h>

#include <util/log.h>
#include <util/mem_utils.h>
#include <grammar/parser.h>
#include <codegen/codegen.h>

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

void dump_bitcode(char* fname, char* dest)
{
	ParserContext* parser = init_parser(fname);
	ASTNode* ast = parse(parser);
	CodegenContext* context = init_codegen();
	
	codegen(context, ast);
	LLVMBuildRetVoid(context->builder);
	
	char *error = NULL;
	LLVMVerifyModule(context->module, LLVMAbortProcessAction, &error);
	LLVMDisposeMessage(error);
	
	LLVMRunFunctionAsMain(context->engine, context->main_func, 0, NULL, NULL);
	
	if (LLVMWriteBitcodeToFile(context->module, dest) != 0)
		log_err("error writing bitcode to file, skipping\n");
	
	destroy_codegen(context);
	destroy_parser(parser);
	destroy_ast_node(ast);
}

void dump_asm(char* fname, char* dest)
{
	dump_bitcode(fname, "/tmp/.bc-tmp");
	char* llc = malloc(100);
	sprintf(llc, "llc /tmp/.bc-tmp -o %s", dest);
	system(llc);
	destroy(llc);
}

void dump_binary(char* fname, char* dest)
{
	dump_asm(fname, "/tmp/.asm-tmp.s");
	char* cc = malloc(100);
	sprintf(cc, "cc /tmp/.asm-tmp.s -o %s", dest);
	system(cc);
	destroy(cc);
}

void dump_ast(char* fname)
{
	ParserContext* parser = init_parser(fname);
	ASTNode* ast = parse(parser);
	ast_dump(ast);
	destroy_parser(parser);
	destroy_ast_node(ast);
}

void dump_ir(char* fname)
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
	
	destroy_codegen(context);
	destroy_parser(parser);
	destroy_ast_node(ast);
}


