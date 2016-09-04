#pragma once

#include "lexer.h"
#include "../util/collections/list.h"

struct ASTNode;

typedef struct {
	TokenType type;
	union {
		Token* token;
	};
} ASTNode;

ASTNode* init_ast_node(List* tokens);
void destroy_ast_node(void* node);
void ast_dump(List* ast);
