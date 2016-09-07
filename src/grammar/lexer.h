#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../util/collections/fifo.h"

typedef enum {
	TOK_EOF,
	TOK_IDENT,
	TOK_INTEGER_LITERAL
} TokenType;

struct Token;

typedef union {
	char* string;
	int64_t* integer;
} TokenValue;

typedef struct {
	size_t line;
	size_t column;
} TokenPos;

typedef struct Token {
	TokenType type;
	TokenValue* val;
	TokenPos pos;
} Token;

extern FIFO* _tokens;

void init_lexer(char* fname);
void destroy_lexer();
void destroy_token();

void lex();
void dump_tokens();

