#include "parser.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "../util/debug.h"
#include "../util/mem_utils.h"
#include "../grammar/ast.h"
#include "../util/regex_utils.h"
#include "../util/collections/fifo.h"

//static const bool FINISHED = true;
static char* _current_file = "";
static List* _current_tokens;
extern FIFO* _tokens;

void init_parser(char* fname)
{
	_current_file = fname;
	init_lexer(_current_file);
	//_ast = init_list_objects(&destroy_ast_node);
	lex();
}

void destroy_parser()
{
	destroy_lexer();
	destroy_list(_current_tokens);
}

static void parse_error(Token* token, const char* fmt, ...)
{
	fprintf(stderr, "%s[ERROR]%s ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
	char msg[250];

	sprintf(msg, "[%s:%zu:%zu] ",
			_current_file, token->pos.line, token->pos.column);
	strcat(msg, fmt);

	va_list args;

	va_start(args, fmt);
	vprintf(msg, args);
	va_end(args);
}

static bool valid_ident(Token* ident)
{
	regex_t regex;
	regex_t regex_underscores;

	const char* regex_text = "[_a-z][_a-z0-9]*";
	const char* regex_text_underscores = "[^_]+";

	compile_regex(&regex, regex_text);
	compile_regex(&regex_underscores, regex_text_underscores);

	if (ident && ident->val && ident->val->string) {
		if (!match_regex(&regex, ident->val->string) ||
				!match_regex(&regex_underscores, ident->val->string)) {
			regfree(&regex_underscores);
			regfree(&regex);
			return false;
		}
	}

	regfree(&regex_underscores);
	regfree(&regex);
	return true;
}

/* Always valid, just pop and return true; */
static bool parse_integer_literal()
{
	fifo_pop(_tokens);

	return true;
}

static bool parse_ident()
{
	Token* ident = fifo_pop(_tokens);

	if (!valid_ident(ident)) {
		parse_error(ident, "not a valid identifier\n");
		destroy_token(ident);
		return false;
	}

	return true;
}

/* Always valid, verified in lexical analysis - just pop */
static bool parse_type()
{
	fifo_pop(_tokens);

	return true;
}


/**
 * Starting point for parser
 *
 * @return Terminate?
 */
static ASTNode* parse_x_lang()
{
	Token* token = (Token*) fifo_peek(_tokens);

	if (token) {
		_current_tokens = init_list_objects(&destroy_token);

		switch (token->type) {
			case TOK_EOF:
				destroy_fifo(_tokens);
				destroy_token(token);

				return NULL;

			case TOK_IDENT:
				if (parse_ident()) {
					list_append(_current_tokens, token);
					return init_ast_node(_current_tokens);
				}

				break;

			case TOK_INTEGER_LITERAL:
				if (parse_integer_literal()) {
					list_append(_current_tokens, token);
					return init_ast_node(_current_tokens);
				}

				break;

			case TOK_TYPE_S8:
			case TOK_TYPE_S16:
			case TOK_TYPE_S32:
			case TOK_TYPE_S64:
			case TOK_TYPE_U8:
			case TOK_TYPE_U16:
			case TOK_TYPE_U32:
			case TOK_TYPE_U64:
				if (parse_type()) {
					list_append(_current_tokens, token);

					Token* ident = fifo_pop(_tokens);

					if (parse_ident())
						list_append(_current_tokens, ident);

					return init_ast_node(_current_tokens);
				}

				break;

			default:
				parse_error(token, 
							"expected one of <ident, integer_literal, EOF>\n");
				return NULL;
		}
	}

	return NULL;
}

List* parse()
{
	List* ast = init_list(&destroy_ast_node);
	
	ASTNode* node = NULL;
	
	while ((node = parse_x_lang()) != NULL)
		list_append(ast, node);
	
	return ast;
}

