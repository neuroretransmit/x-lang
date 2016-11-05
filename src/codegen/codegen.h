#pragma once

#include <llvm-c/Core.h>
#include <grammar/ast.h>
#include <util/collections/hashmap.h>

typedef struct {
	LLVMModuleRef module;
	LLVMBuilderRef builder;
	LLVMPassManagerRef pass_manager;
	HashMap* named_values;
} CodegenContext;

typedef struct {
	char* ident;
	LLVMValueRef val;
} NamedValue;

CodegenContext* init_codegen();
void destroy_codegen(CodegenContext* context);
LLVMValueRef codegen(CodegenContext* context, ASTNode* ast);
