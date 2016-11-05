#include <codegen/codegen.h>

#include <stdlib.h>
#include <stdbool.h>

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

static LLVMValueRef codegen_var_decl(CodegenContext* context, ASTNode* node)
{
	if (node) {
		LLVMValueRef var = LLVMGetNamedGlobal(context->module, node->var_decl->ident->val->string);
	
		if (var) {
			log_err("variable already exists with the same name\n");
		} else {
			LLVMTypeRef type = 	deduce_llvm_type(node->var_decl->type->type);
			LLVMValueRef decl = LLVMAddGlobal(context->module, type, node->var_decl->ident->val->string);
		
			return decl;
		}
	}
	
	return NULL;
}

LLVMValueRef codegen(CodegenContext* context, ASTNode* node)
{
	LLVMValueRef root = NULL;
	
	
	if (node) {
		switch (node->type) {
			case AST_TYPE_ROOT:
				for (size_t i = 0; i < node->children->size; i++) {
					// TODO: get child
					ASTNode* child = list_get(node->children, i);
					
					if (child) {
						root = codegen(context, child);
					}
					
				}
				break;
			case AST_TYPE_VARIABLE_DECLARATION:
				root = codegen_var_decl(context, node);
				break;
			default:
				log_warn("no case for this AST type %d.\n", node->type);
				break;
		}
	}
	
	return root;
}

CodegenContext* init_codegen()
{
	CodegenContext* context = malloc(sizeof(CodegenContext));
	context->root_context = LLVMGetGlobalContext();
	context->module = LLVMModuleCreateWithName("__x_lang");
	context->builder = LLVMCreateBuilderInContext(context->root_context);
	LLVMTypeRef main_type = LLVMFunctionType(LLVMVoidType(), NULL, 0, 0);
	LLVMValueRef main_func = LLVMAddFunction(context->module, "main", main_type);
	context->entry = LLVMAppendBasicBlock(main_func, "entry");
	LLVMPositionBuilderAtEnd(context->builder, context->entry);
	LLVMInitializeNativeTarget();
	
	return context;
}

void destroy_codegen(CodegenContext* context)
{
	if (context) {
		LLVMDisposeModule(context->module);
		LLVMDisposeBuilder(context->builder);
		
		destroy(context);
	}
}
