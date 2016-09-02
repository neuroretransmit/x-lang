#pragma once

#include "lexer.h"
#include "../util/collections/list.h"

struct ASTNode;

typedef struct {
	TokenType type;
	union {
		Token token;
	};
} ASTNode;

ASTNode* init_ast_token(TokenType type, Token token);
void destroy_ast_token(ASTNode* node);
void ast_dump(List* ast);


