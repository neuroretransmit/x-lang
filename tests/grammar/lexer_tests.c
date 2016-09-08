#include "lexer_tests.h"

#include <inttypes.h>
#include <assert.h>
#include <string.h>

#include <grammar/lexer.h>
#include <util/file_utils.h>
#include <util/log.h>

extern FIFO* _tokens;

static void ident_test()
{
	char* fname = "res/lexer_tests.x";
	
	if (file_exists(fname))
		init_lexer(fname);
	else
		log_kill("test file does not exist");
	lex();

	const char* expected[] = {
		"_", "__", "a", "a0", "_0", "A0", "a000"
	};

	while (_tokens->size) {
		Token* token = fifo_pop(_tokens);

		if (token->type != TOK_EOF)
			assert(strcmp(token->val->string, expected[_tokens->size - 1]));

		destroy_token(token);
	}

	log_info("PASS\n");
	destroy_lexer();
}

static void integer_literal_test()
{
	char* fname = "res/integer_literal.x";
	
	if (file_exists(fname))
		init_lexer(fname);
	else
		log_kill("test file does not exist");
	
	lex();

	const int64_t expected[] = {
		0, 66, 23976, 238, 129238, 000
	};

	for (size_t i = 0; _tokens->size > 0; i++) {
		Token* token = fifo_pop(_tokens);

		if (token->type != TOK_EOF)
			assert(*token->val->integer == expected[i]);

		destroy_token(token);

	}

	log_info("PASS\n");
	destroy_lexer();
}

static void type_test()
{
	char* fname = "res/type.x";
	
	if (file_exists(fname))
		init_lexer(fname);
	else
		log_kill("test file does not exist");
	
	lex();

	const unsigned EXPECTED[] = {
		TOK_TYPE_U8, TOK_TYPE_U16, TOK_TYPE_U32, TOK_TYPE_U64,
		TOK_TYPE_S8, TOK_TYPE_S16, TOK_TYPE_S32, TOK_TYPE_S64
	};

	for (size_t i = 0; _tokens->size > 0; i++) {
		Token* token = fifo_pop(_tokens);

		if (token->type != TOK_EOF)
			assert(token->type == EXPECTED[i]);

		destroy_token(token);

	}

	log_info("PASS\n");
	destroy_lexer();
}

void lexer_tests()
{
	ident_test();
	integer_literal_test();
	type_test();
}
