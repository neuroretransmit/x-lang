#include "lexer_tests.h"

#include <assert.h>
#include <string.h>

#include <grammar/lexer.h>
#include <util/log.h>

extern FIFO* _tokens;

// TODO: Restrict dumb names like _ and __
static void ident_test()
{
	init_lexer("res/lexer.x");
	lex();
	
	const char* expected[] = {
		"_", "__", "a", "A", "a0", "_0", "A0", "a000"
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

void lexer_tests()
{
	ident_test();
}
