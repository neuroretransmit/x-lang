#include <grammar/ast.h>

#include <inttypes.h>
#include <stdlib.h>

#include <util/mem_utils.h>
#include <util/log.h>
#include <grammar/lexer.h>

static ASTNode* _allocate_node(Token* tok)
{
	ASTNode* node = malloc(sizeof(ASTNode));
	node->token = tok;
	node->type = tok->type;

	return node;
}

static ASTNode* init_ast_ident(Token* tok)
{
	return _allocate_node(tok);
}

static ASTNode* init_ast_integer_literal(Token* tok)
{
	return _allocate_node(tok);
}

static ASTNode* init_ast_variable_declaration(ASTType ast_type, Token* type, Token* ident)
{
	ASTNode* node = malloc(sizeof(ASTNode));
	ASTVariableDeclaration* var_decl = malloc(sizeof(ASTVariableDeclaration));
	node->type = ast_type;
	var_decl->ident = ident;
	var_decl->type = type;
	node->var_decl = var_decl;

	return node;
}

static ASTNode* init_ast_variable_definition(ASTType ast_type, ASTNode* var_decl, Token* equal, Token* val)
{
	ASTNode* node = NULL;
	
	if (var_decl && equal && val && ast_type == AST_TYPE_VARIABLE_DEFINITION) {
		node = malloc(sizeof(ASTNode));
		ASTVariableDefinition* var_def = malloc(sizeof(ASTVariableDefinition));
		var_def->decl = var_decl->var_decl;
		node->type = ast_type;
		
		switch (val->type) {
			case TOK_INTEGER_LITERAL:
				var_def->val = val;
				node->var_def = var_def;
				break;
			default:
				log_warn("unimplemented variable definition type\n");
		}
	}
	
	return node;
}

ASTNode* init_ast_node(List* tokens)
{
	Token* token = list_get(tokens, 0);
	ASTNode* node = NULL;
	
	if (token) {
		switch (token->type) {
			case TOK_IDENT:
				node = init_ast_ident(token);
				break;

			case TOK_INTEGER_LITERAL:
				node = init_ast_integer_literal(token);
				break;

			case TOK_TYPE_S8:
			case TOK_TYPE_S16:
			case TOK_TYPE_S32:
			case TOK_TYPE_S64:
			case TOK_TYPE_U8:
			case TOK_TYPE_U16:
			case TOK_TYPE_U32:
			case TOK_TYPE_U64: {
				Token* ident = list_get(tokens, 1);
				
				if (ident->type != TOK_IDENT) {
					log_err("expected identifier\n");
					break;
				} else {
					node = init_ast_variable_declaration(AST_TYPE_VARIABLE_DECLARATION, token, ident);
					
					if (tokens->size == 4) {
						Token* equal = list_get(tokens, 2);
						Token* val = list_get(tokens, 3);
						node = init_ast_variable_definition(AST_TYPE_VARIABLE_DEFINITION, node, equal, val);
					}
				}
				
				break;
			}

			default:
				log_err("found token type %d...\n", token->type);
				break;
		}
	}

	return node;
}

static void destroy_ast_variable_declaration(ASTVariableDeclaration* var_decl)
{
	if (var_decl) {
		if (var_decl->ident)
			destroy_token(var_decl->ident);

		if (var_decl->type)
			destroy_token(var_decl->type);

		destroy(var_decl);
	}
}

static void destroy_ast_variable_definition(ASTVariableDefinition* var_def)
{
	if (var_def) {
		if (var_def->decl)
			destroy_ast_variable_declaration(var_def->decl);
		if (var_def->val)
			destroy_token(var_def->val);
		
		destroy(var_def);
	}
}

void destroy_ast_node(void* node)
{
	ASTNode* converted = (ASTNode*) node;

	if (converted) {
		switch (converted->type) {
			case AST_TYPE_ROOT:
				destroy_list(converted->children);
				break;
			case AST_TYPE_VARIABLE_DECLARATION:
				destroy_ast_variable_declaration(converted->var_decl);
				break;
			case AST_TYPE_VARIABLE_DEFINITION:
				destroy_ast_variable_definition(converted->var_def);
				break;
			default:
				break;
		}
		
		destroy(converted);
	}
}

static void print_depth(int depth)
{
	for (int i = 0; i < depth; i++)
		printf("  ");
}

static void ast_dump_ident(Token* ident, int depth)
{
	if (ident) {
		print_depth(depth);
		printf("<%zu:%zu:ident:\"%s\">\n",
			   ident->pos.line, ident->pos.column,
			   ident->val->string);
	}
}

static void ast_dump_integer_literal(Token* integer_literal, int depth)
{
	if (integer_literal) {
		print_depth(depth);
		printf("<%zu:%zu:integer_literal:%" PRId64 ">\n",
			   integer_literal->pos.line, integer_literal->pos.column,
			   *integer_literal->val->integer);
	}
}

static void ast_dump_type(Token* type, int depth)
{
	if (type) {
		print_depth(depth);
		printf("<%zu:%zu:type:%s>\n",
			   type->pos.line, type->pos.column,
			   (type->type == TOK_TYPE_S8 ? "s8" :
				type->type == TOK_TYPE_S16 ? "s16" :
				type->type == TOK_TYPE_S32 ? "s32" :
				type->type == TOK_TYPE_S64 ? "s64" :
				type->type == TOK_TYPE_U8 ? "u8" :
				type->type == TOK_TYPE_U16 ? "u16" :
				type->type == TOK_TYPE_U32 ? "u32" :
				type->type == TOK_TYPE_U64 ? "u64" : "not a type"));
	}
}

static void ast_dump_variable_declaration(ASTVariableDeclaration* var_decl, int depth)
{
	if (var_decl) {
		print_depth(depth);

		if (var_decl->type) {
			printf("<%zu:%zu:variable_declaration>\n",
				   var_decl->type->pos.line, var_decl->type->pos.column);
			depth += 1;

			ast_dump_type(var_decl->type, depth);

			if (var_decl->ident)
				ast_dump_ident(var_decl->ident, depth);
		}
	}
}

static void ast_dump_variable_definition(ASTVariableDefinition* var_def, int depth)
{
	if (var_def) {
		print_depth(depth);
		
		if (var_def->decl->type) {
			printf("<%zu:%zu:variable_definition>\n",
				   var_def->decl->type->pos.line, var_def->decl->type->pos.column);
			depth += 1;
			
			ast_dump_type(var_def->decl->type, depth);
			
			if (var_def->decl->ident)
				ast_dump_ident(var_def->decl->ident, depth);
			
			if (var_def->val) {
				switch (var_def->val->type) {
					case TOK_INTEGER_LITERAL:
						ast_dump_integer_literal(var_def->val, depth);
						break;
					default:
						break;
				}
			}
		}
	}
}

void ast_dump(ASTNode* ast)
{
	int depth = 0;

	if (ast->children) {
		for (unsigned i = 0; i < ast->children->size; i++) {
			ASTNode* node = list_get(ast->children, i);

			if (node && node->token) {
				switch (node->type) {
					case TOK_INTEGER_LITERAL:
						ast_dump_integer_literal(node->token, depth);
						break;

					case TOK_IDENT:
						ast_dump_ident(node->token, depth);
						break;

					case TOK_TYPE_S8:
					case TOK_TYPE_S16:
					case TOK_TYPE_S32:
					case TOK_TYPE_S64:
					case TOK_TYPE_U8:
					case TOK_TYPE_U16:
					case TOK_TYPE_U32:
					case TOK_TYPE_U64:
						ast_dump_type(node->token, depth);
						break;

					case AST_TYPE_VARIABLE_DECLARATION:
						ast_dump_variable_declaration(node->var_decl, depth);
						break;

					case AST_TYPE_VARIABLE_DEFINITION:
						ast_dump_variable_definition(node->var_def, depth);
						break;
					default:
						log_err("unsupported AST node type\n");
						break;
				}
			}
		}
	}
}
