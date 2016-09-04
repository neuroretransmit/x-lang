#include "ast.h"

#include <stdlib.h>

#include "../util/log.h"
#include "../grammar/lexer.h"

static ASTNode* init_ast_ident(Token* tok)
{
	ASTNode* node = malloc(sizeof(ASTNode));
	node->token = tok;
	
	return node;
}

ASTNode* init_ast_node(List* tokens)
{
	for (unsigned i = 0; i < tokens->size; i++) {
		Token* token = list_get(tokens, i);
		if (token) {
			switch (token->type) {
				case TOK_IDENT:
					return init_ast_ident(token);
				default:
					log_err("expected one of <ident>, ...\n");
					return NULL;
			}
		}
	}
	
	return NULL;
}

void destroy_ast_node(void* node)
{
	if (node) {
		free(node);
		node = NULL;
	}
}

static void print_depth(int depth)
{
	for (int i = 0; i < depth; i++)
			printf("  ");
}

static void ast_dump_ident(ASTNode* ident, int depth)
{
	if (ident) {
		print_depth(depth);
		printf("<%zu:%zu:ident:\"%s\">\n", 
				ident->token->pos.line, ident->token->pos.column,
				ident->token->val->string);
	}
}

void ast_dump(List* ast)
{
	int depth = 0;
	
	for (unsigned i = 0; i < ast->size; i++) {
		ASTNode* node = list_get(ast, i);
		
		switch (node->token->type) {
			case TOK_EOF:
				break;
			case TOK_IDENT:
				ast_dump_ident(node, depth);
				break;
		}
	}
}