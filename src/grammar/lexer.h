#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include <util/collections/fifo.h>

/* Reserve the top 100 for root ast constructs */
typedef enum {
	TOK_IDENT = 100,
    
    TOK_CHAR_LITERAL,
	TOK_INTEGER_LITERAL,
    
	TOK_TYPE_S8,
	TOK_TYPE_S16,
	TOK_TYPE_S32,
	TOK_TYPE_S64,
	TOK_TYPE_U8,
	TOK_TYPE_U16,
	TOK_TYPE_U32,
	TOK_TYPE_U64,
    TOK_TYPE_CHAR,
    
    TOK_EQUAL
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
	size_t len;
} Token;

typedef struct {
	char* fname;
	FILE* fp;
	char lookahead;
	char previous;
	TokenPos current_pos;
	TokenPos start;
	FIFO* tokens;
} LexerContext;

LexerContext* init_lexer(char* fname);
TokenValue* init_token_value(TokenType type);
void destroy_lexer();
void destroy_token(void* tok);
void destroy_token_value(TokenType type, void* tok_val);

void lex(LexerContext* lexer);
void dump_tokens();

