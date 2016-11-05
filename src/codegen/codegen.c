#include <codegen/codegen.h>

#include <stdlib.h>

#include <llvm-c/Target.h>

#include <util/mem_utils.h>

CodegenContext* init_codegen()
{
	CodegenContext* context = malloc(sizeof(CodegenContext));
	context->module = LLVMModuleCreateWithName("x-lang");
	context->builder = LLVMCreateBuilder();
	context->named_values = init_hashmap(NULL, NULL);
	LLVMInitializeNativeTarget();
	
	return context;
}


void destroy_codegen(CodegenContext* context)
{
	if (context) {
		LLVMDisposeModule(context->module);
		LLVMDisposeBuilder(context->builder);
		destroy_hashmap(context->named_values);
		
		destroy(context);
	}
}
