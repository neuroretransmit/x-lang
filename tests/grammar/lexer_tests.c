#include "lexer_tests.h"

#include <inttypes.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include <grammar/lexer.h>
#include <util/file_utils.h>
#include <util/mem_utils.h>
#include <util/log.h>

#include "helper.h"

#define MATCH 0

static LexerContext* setup(char* fname)
{
	srand(time(NULL));
	
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

	char* EXPECTED_VALUES[] = {
		"ii", "x", "b", "a", "_a", "__a_", "xylophone_dreams"
	};
	
	Token* EXPECTED[7] = {
		mock_token(TOK_IDENT,
			mock_token_value(TOK_IDENT, strdup(EXPECTED_VALUES[0])),
			mock_token_pos(1, 1),
			2
		),
		mock_token(TOK_IDENT,
			mock_token_value(TOK_IDENT, strdup(EXPECTED_VALUES[1])),
			mock_token_pos(2, 1),
			1
		),
		mock_token(TOK_IDENT,
			mock_token_value(TOK_IDENT, strdup(EXPECTED_VALUES[2])),
			mock_token_pos(3, 1),
			1
		),
		mock_token(TOK_IDENT,
			mock_token_value(TOK_IDENT, strdup(EXPECTED_VALUES[3])),
			mock_token_pos(4, 1),
			1
		),
		mock_token(TOK_IDENT,
			mock_token_value(TOK_IDENT, strdup(EXPECTED_VALUES[4])),
			mock_token_pos(5, 1),
			2
		),
		mock_token(TOK_IDENT,
			mock_token_value(TOK_IDENT,  strdup(EXPECTED_VALUES[5])),
			mock_token_pos(6, 1),
			4
		),
		mock_token(TOK_IDENT,
			mock_token_value(TOK_IDENT,  strdup(EXPECTED_VALUES[6])),
			mock_token_pos(7, 1),
			16
		)
	};
	
	Token* token = NULL;
	
	for (unsigned i = 0; lexer->tokens->size; i++) {
		token = (Token*) fifo_pop(lexer->tokens);
		check_token(token, EXPECTED[i]);
		destroy_token(token);
		destroy_token(EXPECTED[i]);
	}
	
	log_info("PASS\n");
	
	destroy_lexer(lexer);
}

static void integer_literal_test()
{
	LexerContext* lexer = setup("res/integer_literal.x");
	lex(lexer);
	
	int64_t _expected[] = { 0, 66, 23976, 238, 129238, 0 };
	
	void** expected = malloc(sizeof(_expected));
	memcpy(expected, _expected, sizeof(_expected));
	
	TokenPos position[6] = {
		mock_token_pos(1, 1),
		mock_token_pos(1, 3),
		mock_token_pos(1, 6),
		mock_token_pos(1, 12),
		mock_token_pos(1, 16),
		mock_token_pos(1, 23)
	};
	
	Token* EXPECTED[6] = {
		mock_token(TOK_INTEGER_LITERAL, mock_token_value(TOK_INTEGER_LITERAL, &expected[0]), position[0], 1),
		mock_token(TOK_INTEGER_LITERAL, mock_token_value(TOK_INTEGER_LITERAL, &expected[1]), position[1], 2),
		mock_token(TOK_INTEGER_LITERAL, mock_token_value(TOK_INTEGER_LITERAL, &expected[2]), position[2], 5),
		mock_token(TOK_INTEGER_LITERAL, mock_token_value(TOK_INTEGER_LITERAL, &expected[3]), position[3], 3),
		mock_token(TOK_INTEGER_LITERAL, mock_token_value(TOK_INTEGER_LITERAL, &expected[4]), position[4], 6),
		mock_token(TOK_INTEGER_LITERAL, mock_token_value(TOK_INTEGER_LITERAL, &expected[5]), position[5], 3)
	};
	
	Token* actual = NULL;
	for (size_t i = 0; i < 6; i++) {
		actual = fifo_pop(lexer->tokens);
		check_token(actual, EXPECTED[i]);	
		/* Weird houdini stuff I don't feel like unraveling -
		 * 	basically the pointer from expected is freed from actual.
		 * 	could dupe... but it's not leaking... */
		destroy_token(actual);
		destroy(EXPECTED[i]->val);
		destroy(EXPECTED[i]);
	}
	
	log_info("PASS\n");
	destroy_lexer(lexer);
	destroy(expected);
}

static void type_test()
{
	LexerContext* lexer = setup("res/type.x");
	lex(lexer);
	
	Token* EXPECTED[8] = {
		mock_token(TOK_TYPE_U8, NULL, mock_token_pos(1, 1), 2),
		mock_token(TOK_TYPE_U16, NULL, mock_token_pos(1, 4), 3),
		mock_token(TOK_TYPE_U32, NULL, mock_token_pos(1, 8), 3),
		mock_token(TOK_TYPE_U64, NULL, mock_token_pos(1, 12), 3),
		mock_token(TOK_TYPE_S8, NULL, mock_token_pos(2, 1), 2),
		mock_token(TOK_TYPE_S16, NULL, mock_token_pos(2, 4), 3),
		mock_token(TOK_TYPE_S32, NULL, mock_token_pos(2, 8), 3),
		mock_token(TOK_TYPE_S64, NULL, mock_token_pos(2, 12), 3)
	};
	
	Token* token = NULL;
	for (size_t i = 0; i < 8; i++) {
		token = fifo_pop(lexer->tokens);
		check_token(token, EXPECTED[i]);
		destroy_token(token);
		destroy_token(EXPECTED[i]);
	}
	
	destroy_lexer(lexer);
	log_info("PASS\n");
}

void lexer_tests()
{
 	ident_test();
	type_test();
	integer_literal_test();
}
