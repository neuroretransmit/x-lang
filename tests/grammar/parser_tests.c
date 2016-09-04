#include "parser_tests.h"

#include <grammar/parser.h>

void parse_ident_test()
{
	init_parser("res/ident.x");
	destroy_parser();
}

void parser_tests()
{
	parse_ident_test();
}