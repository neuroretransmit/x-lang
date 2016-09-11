#include "lexer_tests.h"

#include <inttypes.h>
#include <assert.h>
#include <string.h>

#include <grammar/lexer.h>
#include <util/file_utils.h>
#include <util/mem_utils.h>
#include <util/log.h>

#include "helper.h"

#define MATCH 0

static LexerContext* setup(char* fname)
{
	LexerContext* lexer;

	if (file_exists(fname))
		lexer = init_lexer(fname);
	else
		log_kill("test file does not exist");

	return lexer;
}

static void ident_test()
{
	LexerContext* lexer = setup("res/ident.x");
	lex(lexer);

	char* EXPECTED_VALUES[7] = {
		"ii", "x", "b", "a", "_a", "__a_", "xylophone_dreams"
	};

	Token* EXPECTED[7] = {
		mock_token(TOK_IDENT,
		mock_token_value(TOK_IDENT, &EXPECTED_VALUES[0]),
		mock_token_pos(1, 1),
		2
				  ),
		mock_token(TOK_IDENT,
		mock_token_value(TOK_IDENT, &EXPECTED_VALUES[1]),
		mock_token_pos(2, 1),
		1
				  ),
		mock_token(TOK_IDENT,
		mock_token_value(TOK_IDENT, &EXPECTED_VALUES[2]),
		mock_token_pos(3, 1),
		1
				  ),
		mock_token(TOK_IDENT,
		mock_token_value(TOK_IDENT, &EXPECTED_VALUES[3]),
		mock_token_pos(4, 1),
		1
				  ),
		mock_token(TOK_IDENT,
		mock_token_value(TOK_IDENT, &EXPECTED_VALUES[4]),
		mock_token_pos(5, 1),
		2
				  ),
		mock_token(TOK_IDENT,
		mock_token_value(TOK_IDENT, &EXPECTED_VALUES[5]),
		mock_token_pos(6, 1),
		4
				  ),
		mock_token(TOK_IDENT,
		mock_token_value(TOK_IDENT, &EXPECTED_VALUES[6]),
		mock_token_pos(7, 1),
		16
				  )
	};
	
	Token* token = NULL;
	
	for (unsigned i = 0; lexer->tokens->size; i++) {
		token = (Token*) fifo_pop(lexer->tokens);
		check_token(token, EXPECTED[i]);
		destroy_token(token);
	}

	log_info("PASS\n");
	destroy_lexer(lexer);
}

static void integer_literal_test()
{
	LexerContext* lexer = setup("res/integer_literal.x");
	lex(lexer);

	int64_t EXPECTED_VALUES[] = {
		0, 66, 23976, 238, 129238, 000
	};

	Token* EXPECTED[7] = {
		mock_token(TOK_INTEGER_LITERAL,
		mock_token_value(TOK_INTEGER_LITERAL, &EXPECTED_VALUES[0]),
		mock_token_pos(1, 1),
		1
				  ),
		mock_token(TOK_INTEGER_LITERAL,
		mock_token_value(TOK_INTEGER_LITERAL, &EXPECTED_VALUES[1]),
		mock_token_pos(1, 3),
		2
				  ),
		mock_token(TOK_INTEGER_LITERAL,
		mock_token_value(TOK_INTEGER_LITERAL, &EXPECTED_VALUES[2]),
		mock_token_pos(1, 6),
		5
				  ),
		mock_token(TOK_INTEGER_LITERAL,
		mock_token_value(TOK_INTEGER_LITERAL, &EXPECTED_VALUES[3]),
		mock_token_pos(1, 12),
		3
				  ),
		mock_token(TOK_INTEGER_LITERAL,
		mock_token_value(TOK_INTEGER_LITERAL, &EXPECTED_VALUES[4]),
		mock_token_pos(1, 16),
		6
				  ),
		mock_token(TOK_INTEGER_LITERAL,
		mock_token_value(TOK_INTEGER_LITERAL, &EXPECTED_VALUES[5]),
		mock_token_pos(1, 23),
		3
				  )
	};

	Token* token = NULL;

	for (size_t i = 0; lexer->tokens->size; i++) {
		token = fifo_pop(lexer->tokens);
		check_token(token, EXPECTED[i]);
		destroy_token(token);
	}

	log_info("PASS\n");
	destroy_lexer(lexer);
}

static void type_test()
{
	LexerContext* lexer = setup("res/type.x");

	unsigned EXPECTED_VALUES[] = {
		TOK_TYPE_U8, TOK_TYPE_U16, TOK_TYPE_U32, TOK_TYPE_U64,
		TOK_TYPE_S8, TOK_TYPE_S16, TOK_TYPE_S32, TOK_TYPE_S64
	};

	Token* EXPECTED[8] = {
		mock_token(EXPECTED_VALUES[0],
		mock_token_value(EXPECTED_VALUES[0], NULL),
		mock_token_pos(1, 1),
		2
				  ),
		mock_token(EXPECTED_VALUES[1],
		mock_token_value(EXPECTED_VALUES[1], NULL),
		mock_token_pos(1, 4),
		3
				  ),
		mock_token(EXPECTED_VALUES[2],
		mock_token_value(EXPECTED_VALUES[2], NULL),
		mock_token_pos(1, 8),
		3
				  ),
		mock_token(EXPECTED_VALUES[3],
		mock_token_value(EXPECTED_VALUES[3], NULL),
		mock_token_pos(1, 12),
		3
				  ),
		mock_token(EXPECTED_VALUES[4],
		mock_token_value(EXPECTED_VALUES[4], NULL),
		mock_token_pos(2, 1),
		2
				  ),
		mock_token(EXPECTED_VALUES[5],
		mock_token_value(EXPECTED_VALUES[5], NULL),
		mock_token_pos(2, 4),
		3
				  ),
		mock_token(EXPECTED_VALUES[6],
		mock_token_value(EXPECTED_VALUES[6], NULL),
		mock_token_pos(2, 8),
		3
				  ),
		mock_token(EXPECTED_VALUES[7],
		mock_token_value(EXPECTED_VALUES[7], NULL),
		mock_token_pos(2, 12),
		3
				  ),

	};

	for (size_t i = 0; lexer->tokens->size; i++) {
		Token* token = fifo_pop(lexer->tokens);
		check_token(token, EXPECTED[i]);
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
