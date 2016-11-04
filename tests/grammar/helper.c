#include "helper.h"

#include <assert.h>
#include <string.h>
#include <util/log.h>
#include <grammar/ast.h>

#define MATCH ( 0 )

static void check_token_val(TokenType type, TokenValue* actual, TokenValue* expected)
{
	if (actual && expected) {
		switch (type) {
			case TOK_IDENT:
				assert(strcmp(actual->string, expected->string) == MATCH);
				break;

			case TOK_INTEGER_LITERAL:
				assert(*actual->integer == *expected->integer);
				break;

			case TOK_TYPE_S8:
			case TOK_TYPE_S16:
			case TOK_TYPE_S32:
			case TOK_TYPE_S64:
			case TOK_TYPE_U8:
			case TOK_TYPE_U16:
			case TOK_TYPE_U32:
			case TOK_TYPE_U64:
				break;

			default:
				log_kill("no case for this token type\n");
				break;
		}
	}
}

static void check_token_pos(TokenPos actual, TokenPos expected)
{
	assert(actual.line == expected.line);
	assert(actual.column == expected.column);
}

void check_token(Token* actual, Token* expected)
{
	if (actual && expected) {
		assert(actual->type == expected->type);
		assert(actual->len == expected->len);
		check_token_pos(actual->pos, expected->pos);

		if (actual->val)
			check_token_val(actual->type, actual->val, expected->val);
	}
}

void check_ast_variable_declaration(ASTVariableDeclaration* actual, ASTVariableDeclaration* expected)
{
	if (actual && expected) {
		if (actual->type)
			check_token(actual->type, expected->type);

		if (actual->ident)
			check_token(actual->ident, actual->ident);
	}
}

void check_ast_node(ASTNode* actual, ASTNode* expected)
{
	if (actual && expected) {
		assert(actual->type == expected->type);
		
		if (actual->var_decl)
			check_ast_variable_declaration(actual->var_decl, expected->var_decl);
		else
			check_token(actual->token, expected->token);

		
	}
}

ASTVariableDeclaration* mock_ast_variable_declaration(Token* type, Token* ident)
{
	ASTVariableDeclaration* var_decl = malloc(sizeof(ASTVariableDeclaration));
	var_decl->type = type;
	var_decl->ident = ident;

	return var_decl;
}

ASTNode* mock_ast_node(ASTType type, void* val)
{
	ASTNode* node = malloc(sizeof(ASTNode));
	node->type = type;

	switch (type) {
		case AST_TYPE_ROOT:
			node->type = AST_TYPE_ROOT;
			break;
		case AST_TYPE_VARIABLE_DECLARATION:
			node->var_decl = val;
			break;

		default:
			log_kill("no case for this token type\n");
			break;
	}

	return node;
}

TokenValue* mock_token_value(TokenType type, void* value)
{
	TokenValue* val = init_token_value(type);
	
	switch (type) {
		case TOK_IDENT:
			val->string = value;
			break;
			
		case TOK_INTEGER_LITERAL:
			val->integer = value;
			break;
			
		default:
			log_kill("no case statement for this type\n");
			break;
	}

	return val;
}

TokenPos mock_token_pos(size_t line, size_t column)
{
	TokenPos pos = { line, column };

	return pos;
}

Token* mock_token(TokenType type, TokenValue* val, TokenPos pos, size_t len)
{
	Token* token = malloc(sizeof(Token));
	token->type = type;
	token->val = val;
	token->pos = pos;
	token->len = len;

	return token;
}
