#include "lexer_tests.h"

#include <assert.h>
#include <string.h>

#include <grammar/lexer.h>
#include <util/log.h>

extern FIFO* _tokens;

static void ident_test()
{
	init_lexer("res/lexer.x");
	lex();
	
	const char* expected[] = {
		"_", "__", "a", "A", "a0", "_0", "A0", "a000"
	};
	
	while (_tokens->size) {
		Token* token = fifo_pop(_tokens);
		assert(strcmp((char*) token->val, expected[_tokens->size - 1]));
		destroy_token(token);
	}
	
	log_info("PASS\n");
	destroy_lexer();
}

void lexer_tests()
{
	ident_test();
}