#pragma once

#include <grammar/ast.h>
#include <grammar/lexer.h>
#include <util/collections/list.h>

typedef struct {
	char* fname;
	List* ast;
	List* current_tokens;
	LexerContext* lexer_context;
} ParserContext;

ParserContext* init_parser(char* fname);
void destroy_parser(ParserContext* context);
ASTNode* parse(ParserContext* context);

