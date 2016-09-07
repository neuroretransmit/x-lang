#include "lexer_tests.h"

#include <inttypes.h>
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
	init_lexer("res/integer_literals.x");
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

void lexer_tests()
{
	ident_test();
	integer_literal_test();
}
