#pragma once

#include <stdint.h>

#include "lexer.h"
#include "../util/collections/list.h"

typedef enum {
	AST_TYPE_NONE,
	AST_TYPE_VARIABLE_DECLARATION
} ASTType;

struct ASTNode;

typedef struct {
	Token* type;
	Token* ident;
	union {
		uint8_t* u8;
		uint16_t* u16;
		uint32_t* u32;
		uint64_t* u64;
		int8_t* s8;
		int16_t* s16;
		int32_t* s32;
		int64_t* s64;
	};
} ASTVariableDeclaration;

typedef struct {
	int type;
	union {
		Token* token;
		ASTVariableDeclaration* variable_declaration;
	};
} ASTNode;

ASTNode* init_ast_node(List* tokens);
void destroy_ast_node(void* node);
void ast_dump(List* ast);
