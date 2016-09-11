#include "lexer_tests.h"

#include <inttypes.h>
#include <assert.h>
#include <string.h>

#include <grammar/lexer.h>
#include <util/file_utils.h>
#include <util/mem_utils.h>
#include <util/log.h>

#define MATCH 0

static void ident_test()
{
	char* fname = "res/ident.x";
	LexerContext* lexer;

	if (file_exists(fname))
		lexer = init_lexer(fname);
	else
		log_kill("test file does not exist");

	lex(lexer);

	const char* expected[] = {
		"ii", "x", "b", "a", "_a", "__a_", "xylophone_dreams"
	};

	Token* token = NULL;
	
	for (unsigned i = 0; lexer->tokens->size; i++) {
		token = (Token*) fifo_pop(lexer->tokens);
		assert(strcmp(token->val->string, expected[i]) == MATCH);
		destroy_token(token);
	}

	log_info("PASS\n");
	destroy_lexer(lexer);
}

static void integer_literal_test()
{
	char* fname = "res/integer_literal.x";
	LexerContext* lexer;

	if (file_exists(fname))
		lexer = init_lexer(fname);
	else
		log_kill("test file does not exist");

	lex(lexer);

	const int64_t expected[] = {
		0, 66, 23976, 238, 129238, 000
	};

	Token* token = NULL;
	
	for (size_t i = 0; lexer->tokens->size; i++) {
		token = fifo_pop(lexer->tokens);
		assert(*token->val->integer == expected[i]);
		destroy_token(token);
	}

	log_info("PASS\n");
	destroy_lexer(lexer);
}

static void type_test()
{
	char* fname = "res/type.x";
	LexerContext* lexer;

	if (file_exists(fname))
		lexer = init_lexer(fname);
	else
		log_kill("test file does not exist");

	lex(lexer);

	const unsigned EXPECTED[] = {
		TOK_TYPE_U8, TOK_TYPE_U16, TOK_TYPE_U32, TOK_TYPE_U64,
		TOK_TYPE_S8, TOK_TYPE_S16, TOK_TYPE_S32, TOK_TYPE_S64
	};

	for (size_t i = 0; lexer->tokens->size; i++) {
		Token* token = fifo_pop(lexer->tokens);
		assert(token->type == EXPECTED[i]);
		destroy_token(token);
	}

	log_info("PASS\n");
	destroy_lexer(lexer);
}

void lexer_tests()
{
	ident_test();
	integer_literal_test();
	type_test();
}
