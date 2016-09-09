#include "ast.h"

#include <inttypes.h>
#include <stdlib.h>

#include "../util/mem_utils.h"
#include "../util/log.h"
#include "../grammar/lexer.h"

static ASTNode* _allocate_node(Token* tok)
{
	ASTNode* node = malloc(sizeof(ASTNode));
	node->token = tok;
	node->type = tok->type;

	return node;
}

/*static ASTNode* init_ast_type(Token* tok)
{
	return _allocate_node(tok);
}*/

static ASTNode* init_ast_ident(Token* tok)
{
	return _allocate_node(tok);
}

static ASTNode* init_ast_integer_literal(Token* tok)
{
	return _allocate_node(tok);
}

static ASTNode* init_ast_type(Token* tok)
{
	return _allocate_node(tok);
}

static ASTNode* init_ast_variable_declaration(Token* type, Token* ident, ASTType ast_type)
{
	ASTNode* node = malloc(sizeof(ASTNode));
	node->type = ast_type;
	node->variable_declaration = malloc(sizeof(ASTVariableDeclaration));
	node->variable_declaration->ident = ident;
	node->variable_declaration->type = type;

	return node;
}

ASTNode* init_ast_node(List* tokens)
{

	Token* token = list_get(tokens, 0);

	if (token) {
		switch (token->type) {
			case TOK_EOF:
				break;

			case TOK_IDENT:
				return init_ast_ident(token);

			case TOK_INTEGER_LITERAL:
				return init_ast_integer_literal(token);

			case TOK_TYPE_S8:
			case TOK_TYPE_S16:
			case TOK_TYPE_S32:
			case TOK_TYPE_S64:
			case TOK_TYPE_U8:
			case TOK_TYPE_U16:
			case TOK_TYPE_U32:
			case TOK_TYPE_U64: {
				Token* ident = list_get(tokens, 1);

				if (ident->type != TOK_IDENT) {
					log_err("expected identifier\n");
					break;
				} else {
					return init_ast_variable_declaration(token, ident, AST_TYPE_VARIABLE_DECLARATION);
				}
			}

			default:
				log_err("expected one of <ident>, ...\n");
				return NULL;
		}
	}


	return NULL;
}

static void destroy_ast_variable_declaration(ASTVariableDeclaration* variable_declaration)
{
	if (variable_declaration) {
		if (variable_declaration->ident)
			destroy_token(variable_declaration->ident);

		if (variable_declaration->type)
			destroy_token(variable_declaration->type);

		destroy(variable_declaration);
	}
}

void destroy_ast_node(void* node)
{
	ASTNode* converted = (ASTNode*) node;

	if (converted) {
		if (converted->token)
			destroy_token(converted->token);
		
		if (converted->variable_declaration)
			destroy_ast_variable_declaration(converted->variable_declaration);
		
		destroy(converted);
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

static void ast_dump_integer_literal(ASTNode* integer_literal, int depth)
{
	if (integer_literal) {
		print_depth(depth);
		printf("<%zu:%zu:integer_literal:%" PRId64 ">\n",
			   integer_literal->token->pos.line, integer_literal->token->pos.column,
			   *integer_literal->token->val->integer);
	}
}

static void ast_dump_type(ASTNode* type, int depth)
{
	if (type) {
		print_depth(depth);
		printf("<%zu:%zu:type:%s>\n",
			   type->token->pos.line, type->token->pos.column,
			   (type->token->type == TOK_TYPE_S8 ? "s8" :
				type->token->type == TOK_TYPE_S16 ? "s16" :
				type->token->type == TOK_TYPE_S32 ? "s32" :
				type->token->type == TOK_TYPE_S64 ? "s64" :
				type->token->type == TOK_TYPE_U8 ? "u8" :
				type->token->type == TOK_TYPE_U16 ? "u16" :
				type->token->type == TOK_TYPE_U32 ? "u32" :
				type->token->type == TOK_TYPE_U64 ? "u64" : "not a type"));
	}
}

static void ast_dump_variable_declaration(ASTNode* variable_declaration, int depth)
{
	if (variable_declaration) {
		print_depth(depth);

		if (variable_declaration->token) {
			printf("<%zu:%zu:variable_declaration>\n",
				   variable_declaration->variable_declaration->type->pos.line, variable_declaration->variable_declaration->type->pos.column);
			depth += 1;

			ASTNode* type = init_ast_type(variable_declaration->variable_declaration->type);
			ast_dump_type(type, depth);
			destroy(type);

			if (variable_declaration->variable_declaration->ident) {
				ASTNode* node = init_ast_ident(variable_declaration->variable_declaration->ident);
				ast_dump_ident(node, depth);
				destroy(node);
			}
		}
	}
}

void ast_dump(List* ast)
{
	int depth = 0;

	for (unsigned i = 0; i < ast->size; i++) {
		ASTNode* node = list_get(ast, i);

		if (node && node->token) {
			switch (node->type) {
				case TOK_EOF:
					break;

				case TOK_INTEGER_LITERAL:
					ast_dump_integer_literal(node, depth);
					break;

				case TOK_IDENT:
					ast_dump_ident(node, depth);
					break;

				case TOK_TYPE_S8:
				case TOK_TYPE_S16:
				case TOK_TYPE_S32:
				case TOK_TYPE_S64:
				case TOK_TYPE_U8:
				case TOK_TYPE_U16:
				case TOK_TYPE_U32:
				case TOK_TYPE_U64:
					ast_dump_type(node, depth);
					break;

				case AST_TYPE_VARIABLE_DECLARATION:
					ast_dump_variable_declaration(node, depth);
					break;

				default:
					log_err("unsupported AST node type\n");
					break;
			}
		}
	}
}
