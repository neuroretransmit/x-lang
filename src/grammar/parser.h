#pragma once

#include "ast.h"
#include "lexer.h"
#include "../util/collections/list.h"

typedef struct {
	char* fname;
	List* ast;
	List* current_tokens;
	LexerContext lexer_context;
} ParserContext;

ParserContext init_parser(char* fname);
void destroy_parser(ParserContext context);
List* parse(ParserContext context, FIFO* tokens);

