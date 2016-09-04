#include "parser.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "../util/debug.h"
#include "../grammar/ast.h"
#include "../util/regex_utils.h"
#include "../util/collections/fifo.h"

static const bool FINISHED = true;
static char* _current_file = "";
static List* _ast;
extern FIFO* _tokens;

void init_parser(char* fname)
{
	_current_file = fname;
	init_lexer(_current_file);
	_ast = init_list_objects(&destroy_ast_node);
	lex();
}

void destroy_parser()
{
	for (unsigned i = 0; i < _ast->size; i++) {
		destroy_ast_node(list_get(_ast, i));
	}
	
	destroy_list(_ast);
	destroy_lexer();
}

static void parse_error(Token* token, const char* fmt, ...)
{
	fprintf(stderr, "%s[ERROR]%s ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
	char msg[250];
	
	sprintf(msg, "[%s:%zu:%zu] ", _current_file, token->pos.line, token->pos.column);
	strcat(msg, fmt);

	va_list args;

	va_start(args, fmt);
	vprintf(msg, args);
	va_end(args);
}

static bool valid_ident(Token* ident)
{
	regex_t r;
	const char* regex_text = "([_a-z])([_a-z0-9])*";

	compile_regex(&r, regex_text);

	if (ident && ident->val && ident->val->string) {
		if (!match_regex(&r, ident->val->string)) {
			regfree(&r);
			return false;
		}
	}

	regfree(&r);
	return true;
}

static Token* parse_ident()
{
	Token* ident = fifo_pop(_tokens);

	if (!valid_ident(ident)) {
		parse_error(ident, "not a valid identifier\n");
		destroy_token(ident);
		return NULL;
	}

	return ident;
}

/**
 * Starting point for parser
 *
 * @return Was EOF found?
 */
static bool parse_x_lang()
{
	Token* token = (Token*) fifo_peek(_tokens);
	List* root_construct = init_list_objects(&destroy_token);
	
	if (token) {
		if (token->type == TOK_EOF) {
			while (_tokens->size)
				destroy_token(fifo_pop(_tokens));
			
			destroy_list(root_construct);
			return true;
		} else {
			switch (token->type) {
				case TOK_IDENT:
					if (parse_ident() != NULL) {
						list_append(root_construct, token);
						ASTNode* node = init_ast_node(root_construct);
						List* ast_list = init_list(&destroy_ast_node);
						list_append(ast_list, node);
						ast_dump(ast_list);
						destroy_list(ast_list);
					}
					break;

				default:
					parse_error(token, "expected one of <ident, EOF>\n");
					break;
			}
			
			destroy_list(root_construct);
			return false;
		}
	}
	
	return false;
}

void parse()
{
	while (parse_x_lang() != FINISHED);
}

