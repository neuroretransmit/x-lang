#include <codegen/codegen.h>

#include <stdlib.h>

#include <llvm-c/Target.h>

#include <util/mem_utils.h>
#include <util/log.h>

static LLVMTypeRef deduce_llvm_type(int type)
{
	switch (type) {
		case TOK_TYPE_S8:
		case TOK_TYPE_U8: return LLVMInt8Type();
		case TOK_TYPE_S16:
		case TOK_TYPE_U16: return LLVMInt16Type();
		case TOK_TYPE_S32:
		case TOK_TYPE_U32: return LLVMInt32Type();
		case TOK_TYPE_S64:
		case TOK_TYPE_U64: return LLVMInt64Type();
		default:
			log_kill("didn't recieve a type.\n");
			break;
	}
	
	return NULL;
}

static LLVMValueRef codegen_var_decl(CodegenContext* context, ASTVariableDeclaration* node)
{
	LLVMValueRef var = LLVMGetNamedGlobal(context->module, node->ident->val->string);
	
	if (var) {
		log_err("variable already exists with the same name\n");
	} else {
		LLVMTypeRef type = deduce_llvm_type(node->type->type);
		return LLVMAddGlobal(context->module, type, node->ident->val->string);
	}
	
	return NULL;
}

LLVMValueRef codegen(CodegenContext* context, ASTNode* node)
{
	LLVMValueRef root = NULL;
	
	if (node->children) {
		for (size_t i = 0; i < node->children->size; i++) {
			ASTNode* node = list_get(node->children, i);
			
			switch (node->type) {
				case AST_TYPE_VARIABLE_DECLARATION:
					root = codegen_var_decl(context, node->var_decl);
					break;
				default:
					log_info("a case statement doesn't exist for that node type.");
					break;
			}
		}
	}
	
	return root;
}

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
