#pragma once

#include <stddef.h>

#include "../util/collections/fifo.h"

typedef enum {
	TOK_IDENT
} TokenType;

struct Token;

typedef union {
	char* string;
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

void init_lexer(const char* fname);
void destroy_lexer();
void destroy_token();

void lex();
void dump_tokens();

