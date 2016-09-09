#include "codegen.h"

#include <llvm-c/Core.h>

#include "grammar/lexer.h"
#include "grammar/ast.h"
#include "util/log.h"

/*LLVMValueRef codegen(ASTNode* node, LLVMModuleRef module, LLVMBuilderRef builder)
{
	switch (node->type) {
		default:
			log_err("unsupported node type\n");
	}

	return NULL;
}*/
