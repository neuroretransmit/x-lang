#pragma once

#include <llvm-c/Core.h>
#include <grammar/ast.h>

typedef struct {
	LLVMContextRef root_context;
	LLVMModuleRef module;
	LLVMBuilderRef builder;
	LLVMPassManagerRef pass_manager;
} CodegenContext;

typedef struct {
	char* ident;
	LLVMValueRef val;
} NamedValue;

CodegenContext* init_codegen();
void destroy_codegen(CodegenContext* context);
LLVMValueRef codegen(CodegenContext* context, ASTNode* ast);
