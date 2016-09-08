#pragma once

#include "grammar/ast.h"
#include <llvm-c/Core.h>

LLVMValueRef codegen(ASTNode* node, LLVMModuleRef module, LLVMBuilderRef builder);
