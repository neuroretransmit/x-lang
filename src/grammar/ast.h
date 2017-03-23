#pragma once

#include <stdint.h>

#include <grammar/lexer.h>
#include <util/collections/list.h>

typedef enum {
	AST_TYPE_NONE,
	AST_TYPE_ROOT,
	AST_TYPE_VARIABLE_DECLARATION,
    AST_TYPE_ASSIGNMENT
} ASTType;

struct ASTNode;

typedef union {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    int8_t s8;
    int16_t s16;
    int32_t s32;
    int64_t s64;
} ASTValue;

typedef struct {
	Token* type;
	Token* ident;
} ASTVariableDeclaration;

typedef struct {
    Token* ident;
    ASTValue* val;
} ASTAssignment;

typedef struct {
	List* children;
	int type;
	union {
		Token* token;
		ASTVariableDeclaration* var_decl;
        ASTAssignment* var_assign;
	};
} ASTNode;

ASTNode* init_ast_node(List* tokens);
void destroy_ast_node(void* node);
void ast_dump(ASTNode* ast);
