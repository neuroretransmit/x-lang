#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/log.h"

static FILE* _current_file;
static char _lookahead;
FIFO* _tokens;

TokenPos _current_pos = { 1, 1 };
TokenPos _current_tok_start = { 1, 1 };

void destroy_token(void* tok)
{
	Token* token = (Token*) tok;

	if (token) {
		if (token->type) {
			switch (token->type) {
				case TOK_EOF:
					break;

				case TOK_IDENT:
					free(token->val->string);
					token->val->string = NULL;
					break;

				default:
					log_err("destroy_token: type not supported\n");
					break;
			}
		}

		if (token->val) {
			free(token->val);
			token->val = NULL;
		}

		free(token);
		token = NULL;
	}
}

void init_lexer(const char* fname)
{
	_current_file = fopen(fname, "r");
	_tokens = init_fifo_objects(destroy_token);
}

void destroy_lexer()
{
	if (_tokens)
		destroy_fifo(_tokens);

	if (_current_file)
		fclose(_current_file);
}

static void next_token()
{
	_lookahead = fgetc(_current_file);
}

static void adjust_position()
{
	if (_lookahead == '\n') {
		++_current_pos.line;
		_current_pos.column = 1;
	} else if (_lookahead == '\t') {
		_current_pos.column += 4;
	} else if (isprint(_lookahead)) {
		_current_pos.column += 1;
	}
}

static bool is_separator()
{
	switch (_lookahead) {
		case '(':
		case ')':
		case ',':
		case '.':
		case ' ':
		case ';':
		case '\t':
		case '\n':
			return true;
	}

	return false;
}

static char* capture_string()
{
	size_t size;
	char* string = NULL;

	fseek(_current_file, -1, SEEK_CUR);
	unsigned long start = ftell(_current_file);

	for (size = 0; !is_separator(); size++)
		next_token();

	fseek(_current_file, start, SEEK_SET);
	string = malloc(size + 1);

	if (fgets(string, size, _current_file) == 0)
		log_kill("capture_string: failed to retrieve string\n");

	string[size] = '\0';

	return string;
}

static inline void save_token_start()
{
	_current_tok_start.line = _current_pos.line;
	_current_tok_start.column = _current_pos.column;
}

static TokenValue* init_token_value()
{
	return calloc(1, sizeof(TokenValue));
}

static Token* create_token(int type, void* val)
{
	Token* token = malloc(sizeof(Token));
	token->val = init_token_value();
	token->type = type;
	token->pos.line = _current_tok_start.line;
	token->pos.column = _current_tok_start.column;

	switch (type) {
		case TOK_IDENT:
			token->val->string = val;
			break;

		default:
			break;
	}

	return token;
}

static void tokenize()
{
	next_token();
	save_token_start();

	switch (_lookahead) {
		case ' ':
		case '\t':
		case '\n':
			return;

		default: {
			switch (_lookahead) {
				case EOF:
					fifo_push(_tokens, create_token(TOK_EOF, NULL));
					return;
			}

			if (isalpha(_lookahead) || _lookahead == '_') {
				char* tmp = capture_string();
				fifo_push(_tokens, create_token(TOK_IDENT, tmp));
			}

			break;
		}
	}
	
	adjust_position();
	
	/* IF-ELSE chain for types/keywords here */
}

void lex()
{
	do {
		tokenize();
	} while (!feof(_current_file));
}
