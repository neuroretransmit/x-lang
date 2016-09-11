#pragma once

#include <grammar/ast.h>
#include <grammar/lexer.h>

void check_token(Token* actual, Token* expected);
Token* mock_token(TokenType type, TokenValue* val, TokenPos pos, size_t len);
TokenValue* mock_token_value(TokenType type, void* value);
TokenPos mock_token_pos(size_t line, size_t column);

void check_ast_node(ASTNode* actual, ASTNode* expected);
ASTNode* mock_ast_node(ASTType type, void* val);
ASTVariableDeclaration* mock_ast_variable_declaration(Token* type, Token* ident);
