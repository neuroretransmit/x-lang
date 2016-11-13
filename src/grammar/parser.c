#include <grammar/parser.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <grammar/ast.h>
#include <grammar/lexer.h>
#include <util/debug.h>
#include <util/mem_utils.h>
#include <util/regex_utils.h>
#include <util/collections/fifo.h>

/**
 * Initialize parser context
 * 
 * 	@param	fname Source file name
 * 	@return	Parser context
 */
ParserContext* init_parser(char* fname)
{
	ParserContext* context = malloc(sizeof(ParserContext));
	context->lexer_context = init_lexer(fname);
	lex(context->lexer_context);

	return context;
}

/**
 * Destroy parser context
 * 
 * 	@param	context The parser context
 */
void destroy_parser(ParserContext* context)
{
	if (context) {		
		if (context->lexer_context)
			destroy_lexer(context->lexer_context);

		destroy(context);
	}
}

/**
 * Custom error logger for the source file at hand since we can't use internal line numbers
 * 
 * 	@param	context The parser context
 * 	@param	token	The current token
 * 	@param	fmt		Message format string
 */
static void log_parser_error(ParserContext* context, Token* token, const char* fmt, ...)
{
	fprintf(stderr, "%s[ERROR]%s ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
	char msg[250];

	sprintf(msg, "[%s:%zu:%zu] ",
			context->fname, token->pos.line, token->pos.column);
	strcat(msg, fmt);

	va_list args;

	va_start(args, fmt);
	vprintf(msg, args);
	va_end(args);
}

/**
 * Validator for identifier tokens
 * 
 * 	@param	ident Current token
 * 	@return	Is valid?
 * 	@see	tests/grammar/parser_tests.c::ident_test()
 */
static bool valid_ident(Token* ident)
{
	regex_t regex;
	regex_t regex_underscores;

	const char* regex_text = "[_a-z][_a-z0-9]*";
	const char* regex_text_underscores = "[^_]+";

	compile_regex(&regex, regex_text);
	compile_regex(&regex_underscores, regex_text_underscores);

	if (ident && ident->val && ident->val->string) {
		if (!match_regex(&regex, ident->val->string) ||
				!match_regex(&regex_underscores, ident->val->string)) {
			regfree(&regex_underscores);
			regfree(&regex);
			return false;
		}
	}

	regfree(&regex_underscores);
	regfree(&regex);
	return true;
}

/**
 * Parse an identifier
 * 
 * 	@param	ident Current token
 * 	@return	Is valid?
 * 	@see	tests/grammar/parser_tests.c::ident_test()
 */
static bool parse_ident(ParserContext* context, Token* ident)
{
	if (!valid_ident(ident)) {
		destroy_token(ident);
		return false;
	}

	list_append(context->current_tokens, ident);
	
	return true;
}

static bool parse_variable_declaration(ParserContext* context)
{
	Token* ident = fifo_pop(context->lexer_context->tokens);
	
	if (parse_ident(context, ident)) {
		list_append(context->current_tokens, ident);
		return true;
	} else {
		log_parser_error(context, ident, "invalid identifier\n");
	}
	
	return false;
}

static bool parse_variable_definition(ParserContext* context)
{
	Token* equal = fifo_peek(context->lexer_context->tokens);
	
	if (equal && equal->type == TOK_EQUAL) {
		equal = fifo_pop(context->lexer_context->tokens);
		Token* integer_literal = fifo_pop(context->lexer_context->tokens);
	
		if (equal->type == TOK_EQUAL && integer_literal->type == TOK_INTEGER_LITERAL) {
			list_append(context->current_tokens, equal);
			list_append(context->current_tokens, integer_literal);
			return true;
		} else {
			log_err("expectected integer literal.\n");
		}
	}
	
	return false;
}

/**
 * Parse parent abstract syntax tree construct
 * 
 * 	@param	context Parser context
 * 	@return	Abstract syntax tree
 * 	@see	tests/grammar/parser_tests.c::parser_test()
 */
static ASTNode* parse_x_lang(ParserContext* context)
{
	ASTNode* node = NULL;
	Token* token = (Token*) fifo_pop(context->lexer_context->tokens);
	if (token) {
		context->current_tokens = init_list(&destroy_ast_node);

		switch (token->type) {

			case TOK_IDENT:
				if (parse_ident(context, token))
					node = init_ast_node(context->current_tokens);

				break;

			case TOK_INTEGER_LITERAL:
					list_append(context->current_tokens, token);
					node = init_ast_node(context->current_tokens);

				break;


			case TOK_TYPE_S8:
			case TOK_TYPE_S16:
			case TOK_TYPE_S32:
			case TOK_TYPE_S64:
			case TOK_TYPE_U8:
			case TOK_TYPE_U16:
			case TOK_TYPE_U32:
			case TOK_TYPE_U64: {
				list_append(context->current_tokens, token); // Append type, verified in lexical analysis

				bool var_decl = parse_variable_declaration(context);
				bool var_def = parse_variable_definition(context);
				
				if ((var_decl && var_def) || (var_decl || var_def))
					node = init_ast_node(context->current_tokens);
				
				break;
			}
			
			default:
				log_parser_error(context, token,
								 "expected one of <ident, integer_literal, EOF>\n");
				break;
		}
	}
	
	return node;
}

/**
 * Main parsing loop
 * 	@param	context Parser context
 * 	@return	Abstract syntax tree
 * 	@see	tests/grammar/parser_tests.c::parser_test()
 */
ASTNode* parse(ParserContext* context)
{
	lex(context->lexer_context);

	ASTNode* node = calloc(1, sizeof(ASTNode));
	node->type = AST_TYPE_ROOT;
	
	for (ASTNode* tmp = parse_x_lang(context); tmp != NULL; tmp = parse_x_lang(context)) {
		if (node->children == NULL)
			node->children = init_list_objects(&destroy_ast_node);
		
		list_append(node->children, tmp);
		destroy_list(context->current_tokens);
	}
	
	return node;
}

