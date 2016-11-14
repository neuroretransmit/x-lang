#pragma once

#include <stdint.h>

#include <grammar/lexer.h>
#include <util/collections/list.h>

typedef enum {
	AST_TYPE_NONE,
	AST_TYPE_ROOT,
	AST_TYPE_VARIABLE_DECLARATION,
	AST_TYPE_VARIABLE_DEFINITION
} ASTType;

struct ASTNode;

typedef struct {
	Token* type;
	Token* ident;
} ASTVariableDeclaration;

typedef struct {
	ASTVariableDeclaration* decl;
	Token* val;
} ASTVariableDefinition;

typedef struct {
	List* children;
	int type;
	union {
		Token* token;
		ASTVariableDeclaration* var_decl;
		ASTVariableDefinition* var_def;
	};
} ASTNode;

ASTNode* init_ast_node(List* tokens);
void destroy_ast_node(void* node);
void ast_dump(ASTNode* ast);
