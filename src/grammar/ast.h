#pragma once

#include <stdint.h>

#include "lexer.h"
#include "../util/collections/list.h"

typedef enum {
	AST_TYPE_NONE,
	AST_TYPE_VARIABLE_DECLARATION
} ASTType;

struct ASTNode;

typedef struct {
	Token* type;
	Token* ident;
} ASTVariableDeclaration;

typedef struct {
	int type;
	union {
		Token* token;
		ASTVariableDeclaration* variable_declaration;
	};
} ASTNode;

ASTNode* init_ast_node(List* tokens);
void destroy_ast_node(void* node);
void ast_dump(List* ast);
