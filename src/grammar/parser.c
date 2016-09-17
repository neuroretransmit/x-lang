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

ParserContext* init_parser(char* fname)
{
	ParserContext* context = malloc(sizeof(ParserContext));
	context->lexer_context = init_lexer(fname);
	lex(context->lexer_context);

	return context;
}

void destroy_parser(ParserContext* context)
{
	destroy_list(context->current_tokens);
	if (context) {
		if (context->lexer_context)
			destroy_lexer(context->lexer_context);

		destroy(context);
	}
}

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

/* Possible FIXME - Always valid */
static bool parse_integer_literal()
{
	return true;
}

static bool parse_ident(Token* ident)
{
	if (!valid_ident(ident)) {
		destroy_token(ident);
		return false;
	}

	return true;
}

/* Always valid, verified in lexical analysis */
static bool parse_type()
{
	return true;
}

static ASTNode* parse_x_lang(ParserContext* context)
{
	debug("hit_x_lang\n");
	ASTNode* node = NULL;
	Token* token = (Token*) fifo_pop(context->lexer_context->tokens);

	if (token) {
		context->current_tokens = init_list_objects(&destroy_token);

		switch (token->type) {

			case TOK_IDENT:
				debug("hit_ident\n");
				if (parse_ident(token)) {
					list_append(context->current_tokens, token);
					node = init_ast_node(context->current_tokens);
				}

				break;

			case TOK_INTEGER_LITERAL:
				debug("hit_integer\n");
				if (parse_integer_literal()) {
					list_append(context->current_tokens, token);
					node = init_ast_node(context->current_tokens);
				}

				break;


			case TOK_TYPE_S8:
			case TOK_TYPE_S16:
			case TOK_TYPE_S32:
			case TOK_TYPE_S64:
			case TOK_TYPE_U8:
			case TOK_TYPE_U16:
			case TOK_TYPE_U32:
			case TOK_TYPE_U64:
				debug("hit_var_decl\n");
				// --- variable declaration
				if (parse_type()) {
					debug("hit_type\n");
					list_append(context->current_tokens, token);

					Token* ident = fifo_pop(context->lexer_context->tokens);
					
					if (parse_ident(ident)) {
						debug("hit_ident\n");
						list_append(context->current_tokens, ident);
						node = init_ast_node(context->current_tokens);
					} else {
						log_parser_error(context, ident, "invalid identifier\n");
					}
				}

				// --- end variable declaration
				break;

			default:
				log_parser_error(context, token,
								 "expected one of <ident, integer_literal, EOF>\n");
				break;
		}
	}

	return node;
}

List* parse(ParserContext* context)
{
	lex(context->lexer_context);
	List* ast = init_list(&destroy_ast_node);

	ASTNode* node = NULL;

	int i = 1;
	while ((node = parse_x_lang(context)) != NULL) {
		debug("node %d parsed\n", i++);
		list_append(ast, node);
	}


	return ast;
}

