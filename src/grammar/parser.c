#include "parser.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "../util/debug.h"
#include "../util/regex_utils.h"
#include "../util/collections/fifo.h"

extern FIFO* _tokens;

static char* _current_file;

void init_parser(const char* fname)
{
	init_lexer(fname);
	lex();
}

void destroy_parser()
{
	destroy_lexer();
}

static void parse_error(Token* token, const char* fmt, ...)
{
	fprintf(stderr, "[%sERROR%s] ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
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
	const char* regex_text = "[_a-z]([_a-z0-9])*";
	
	compile_regex(&r, regex_text);
	
	if (ident && ident->val && ident->val->string && !match_regex(&r, ident->val->string)) {
		parse_error(ident, "not a valid identifier\n");
		
		regfree(&r);
		return false;
	}
	
	regfree(&r);
	return true;
}

static Token* parse_ident()
{
	Token* ident = fifo_pop(_tokens);

	if (!valid_ident(ident)) {
		parse_error(ident, "parse_ident: not a valid ident\n");
		destroy_token(ident);
		return NULL;
	}
	
	return ident;
}

static bool parse_x_lang()
{
	Token* token = (Token*) fifo_peek(_tokens);
	
	if (token) {
		if (token->type == TOK_EOF) {
			while(_tokens->size)
				destroy_token(fifo_pop(_tokens));
			return true;
		} else {	
			switch (token->type) {
				case TOK_IDENT: {
					parse_ident();
					log_info("%s\n", token->val->string);
					destroy_token(token);
					break;
				}
				default:
					log_err("expected one of <ident, EOF>\n");
					break;
			}
		}
	}
	return false;
}

void parse(char* fname)
{
	_current_file = fname;

	while(!parse_x_lang());
}

