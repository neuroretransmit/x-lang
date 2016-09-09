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

static ASTNode* init_ast_variable_declaration(List* tok, ASTType type)
{
	ASTNode* node = malloc(sizeof(ASTNode) * tok->size);
	node->type = type;

	switch (type) {
		case AST_TYPE_VARIABLE_DECLARATION: {
			if (tok->size != 2)
				log_err("list held too many tokens");

			enum {
				TYPE_IDX,
				IDENT_IDX
			};

			Token* type = list_get(tok, TYPE_IDX);
			Token* ident = list_get(tok, IDENT_IDX);

			if (type && ident) {
				ASTVariableDeclaration* variable_declaration = malloc(sizeof(ASTVariableDeclaration));
				variable_declaration->ident = ident;
				variable_declaration->type = type;
				node->variable_declaration = variable_declaration;
			}

		}

		default:
			break;
	}

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
				List* var_decl_tokens = init_list_objects(&destroy_token);
				list_append(var_decl_tokens, token);
				Token* ident = list_get(tokens, 1);

				if (ident->type != TOK_IDENT) {
					log_err("expected identifier\n");
				} else {
					list_append(var_decl_tokens, ident);
					return init_ast_variable_declaration(var_decl_tokens, AST_TYPE_VARIABLE_DECLARATION); /**/
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

		/*if (variable_declaration->s8)
			destroy(variable_declaration->s8);
		else if (variable_declaration->s16)
			destroy(variable_declaration->s16);
		else if (variable_declaration->s32)
			destroy(variable_declaration->s32);
		else if (variable_declaration->s64)
			destroy(variable_declaration->s64);
		else if (variable_declaration->u8)
			destroy(variable_declaration->u8);
		else if (variable_declaration->u16)
			destroy(variable_declaration->u16);
		else if (variable_declaration->u32)
			destroy(variable_declaration->u32);
		else if (variable_declaration->u64)
			destroy(variable_declaration->u64);
		*/
		
		destroy(variable_declaration);
	}
}

void destroy_ast_node(void* node)
{
	ASTNode* converted = (ASTNode*) node;

	if (converted) {

		switch (converted->type) {
			case TOK_IDENT:
			case TOK_INTEGER_LITERAL:
			case TOK_TYPE_S8:
			case TOK_TYPE_S16:
			case TOK_TYPE_S32:
			case TOK_TYPE_S64:
			case TOK_TYPE_U8:
			case TOK_TYPE_U16:
			case TOK_TYPE_U32:
			case TOK_TYPE_U64:
				if (converted->token)
					destroy_token(converted->token);

				break;

			case AST_TYPE_VARIABLE_DECLARATION:
				if (converted->variable_declaration)
					destroy_ast_variable_declaration(converted->variable_declaration);

				break;

			default:
				log_err("unsupported ast type\n");
		}
		
		if (converted)
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
			destroy_ast_node(type);
			if (variable_declaration->variable_declaration->ident) {
				ASTNode* node = init_ast_ident(variable_declaration->variable_declaration->ident);
				ast_dump_ident(node, depth);
				destroy_ast_node(node);
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
