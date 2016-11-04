#include "parser_tests.h"

#include <string.h>

#include <grammar/lexer.h>
#include <grammar/parser.h>
#include <util/file_utils.h>
#include <util/mem_utils.h>
#include <util/log.h>

#include "helper.h"

static ParserContext* setup(char* fname)
{
	ParserContext* parser;

	if (file_exists(fname))
		parser = init_parser(fname);
	else
		log_kill("test file does not exist");

	return parser;
}

void variable_declaration_test()
{
	ParserContext* parser = setup("res/variable_declaration.x");
	
	ASTNode* ast = parse(parser);

	ASTNode* EXPECTED = mock_ast_node(AST_TYPE_ROOT, NULL);
	ASTNode* tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
								 mock_ast_variable_declaration(
									mock_token(TOK_TYPE_U8, NULL, mock_token_pos(1, 1), 2),
									mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("a_fdf")), mock_token_pos(1, 4), 5)));
	
	EXPECTED->children = init_list_objects(&destroy_ast_node);
	list_append(EXPECTED->children, tmp);
	tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
						mock_ast_variable_declaration(
							mock_token(TOK_TYPE_U16, NULL, mock_token_pos(2, 1), 3),
							mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("x__")), mock_token_pos(2, 5), 3)));
	list_append(EXPECTED->children, tmp);
	tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
						mock_ast_variable_declaration(
							mock_token(TOK_TYPE_U32, NULL, mock_token_pos(3, 1), 3),
							mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("y123")), mock_token_pos(3, 5), 4)));
	list_append(EXPECTED->children, tmp);
	tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
						mock_ast_variable_declaration(
							mock_token(TOK_TYPE_U64, NULL, mock_token_pos(4, 1), 3), 
							mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("z34")), mock_token_pos(4, 5), 3)));
	list_append(EXPECTED->children, tmp);
	tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
						mock_ast_variable_declaration(
							mock_token(TOK_TYPE_S8, NULL, mock_token_pos(5, 1), 2), 
							mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("zwer")), mock_token_pos(5, 4), 4)));
	list_append(EXPECTED->children, tmp);
	tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
						mock_ast_variable_declaration(
							mock_token(TOK_TYPE_S16, NULL, mock_token_pos(6, 1), 3),
							mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("e324")), mock_token_pos(6, 5), 1)));
	list_append(EXPECTED->children, tmp);
	tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
						mock_ast_variable_declaration(
							mock_token(TOK_TYPE_S32, NULL, mock_token_pos(7, 1), 3),
							mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("iwe")), mock_token_pos(7, 5), 3)));
	list_append(EXPECTED->children, tmp);
	tmp = mock_ast_node(AST_TYPE_VARIABLE_DECLARATION,
						mock_ast_variable_declaration(
							mock_token(TOK_TYPE_S64, NULL, mock_token_pos(8, 1), 3),
							mock_token(TOK_IDENT, mock_token_value(TOK_IDENT, strdup("sdif")), mock_token_pos(8, 5), 4)));
	list_append(EXPECTED->children, tmp);
	
	for (size_t i = 0; i < ast->children->size; i++) {
		check_ast_node(list_get(ast->children, i), list_get(EXPECTED->children, i));
	}
	
	log_info("PASS\n");
	
	destroy_ast_node(ast);
	destroy_ast_node(EXPECTED);
	destroy_parser(parser);
}

void parser_tests()
{
	variable_declaration_test();
}
