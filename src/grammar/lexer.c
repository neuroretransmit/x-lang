#include <grammar/lexer.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <util/log.h>
#include <util/mem_utils.h>

#define WHITESPACE ( -1 )
#define FAKE_TOKEN ( (Token*) WHITESPACE )

typedef struct {
	size_t len;
	char* str;
} StringCapture;

void destroy_token_value(void* tok_val)
{
	TokenValue* value = (TokenValue*) tok_val;

	if (value) {
		// LOLWUT - Why is this segfaulting....
		if (value->string)
			destroy(value->string);
		else if (value->integer)
			destroy(value->integer);
		
		destroy(value);
	}
}

void destroy_token(void* token)
{

	if (token) {
		destroy_token_value(((Token*) token)->val);

		destroy(token);
	}
}

LexerContext* init_lexer(char* fname)
{
	LexerContext* context = malloc(sizeof(LexerContext));
	context->fname = fname;
	context->fp = fopen(fname, "r");
	context->tokens = init_fifo_objects(&destroy_token);
	context->start.column = context->current_pos.column = 1;
	context->start.line = context->current_pos.line = 1;
	return context;
}

void destroy_lexer(LexerContext* context)
{
	if (context) {
		if (context->fp)
			fclose(context->fp);

		if (context->tokens)
			destroy_fifo(context->tokens);
		
		destroy(context);
	}
}

static void log_lexer_error(LexerContext* context, const char* fmt, ...)
{
	fprintf(stderr, "%s[ERROR]%s ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
	char msg[250];

	sprintf(msg, "[%s:%zu:%zu] ", context->fname, context->current_pos.line, context->current_pos.column);
	strcat(msg, fmt);

	va_list args;

	va_start(args, fmt);
	vprintf(msg, args);
	va_end(args);
}

static void next_token(LexerContext* context)
{
	context->previous = context->lookahead;
	context->lookahead = fgetc(context->fp);
}

static void adjust_position(LexerContext* context, size_t tok_len)
{   
    if (tok_len == (size_t) -1) {
        ++context->current_pos.line;
        context->current_pos.column = 1;
    } else if (isprint(context->lookahead)) {
		if (isdigit(context->previous)) {
			context->current_pos.column += tok_len;
		} else {
			context->current_pos.column += tok_len;
		}
	}
}

static bool is_separator(LexerContext* context)
{
	switch (context->lookahead) {
		case '(':
		case ')':
		case ',':
		case '.':
		case ' ':
		case ';':
		case '\t':
		case '\n':
			return true;
	}

	return false;
}

void destroy_string_capture(StringCapture* capture)
{
	if (capture) {
		destroy(capture->str);
		destroy(capture);
	}
}

static StringCapture* capture_string(LexerContext* context)
{
	StringCapture* capture = malloc(sizeof(StringCapture));
	size_t len = 0;
	
	fseek(context->fp, -1, SEEK_CUR);
	unsigned long start = ftell(context->fp);

	for (len = 0; !is_separator(context); len++)
		next_token(context);

	fseek(context->fp, start, SEEK_SET);
	capture->str = malloc(len + 1);

	if (fgets(capture->str, len, context->fp) == 0)
		log_kill("failed to retrieve string\n");

	capture->str[len] = '\0';
	capture->len = strlen(capture->str);
	
	return capture;
}

static inline void save_token_start(LexerContext* context)
{
	context->start.line = context->current_pos.line;
	context->start.column = context->current_pos.column;
}

TokenValue* init_token_value(TokenType type)
{
	TokenValue* val = NULL;

	switch (type) {
		case TOK_TYPE_S8:
		case TOK_TYPE_S16:
		case TOK_TYPE_S32:
		case TOK_TYPE_S64:
		case TOK_TYPE_U8:
		case TOK_TYPE_U16:
		case TOK_TYPE_U32:
		case TOK_TYPE_U64:
			break;
			
		case TOK_INTEGER_LITERAL:
			val = malloc(sizeof(TokenValue));
			break;
		case TOK_IDENT:
			val = malloc(sizeof(TokenValue));
			break;

		default:
			log_err("unknown token type\n");
			break;
	}

	return val;
}

static Token* create_token(LexerContext* context, TokenType type, void* val, size_t len)
{
	Token* token = malloc(sizeof(Token));
	token->val = init_token_value(type);
	token->type = type;
	token->pos.line = context->start.line;
	token->pos.column = context->start.column;
	token->len = len;

	switch (type) {
		case TOK_INTEGER_LITERAL:
			token->val->integer = val;
			break;

		case TOK_IDENT:
			token->val->string = val;
			break;

		case TOK_TYPE_S8:
		case TOK_TYPE_S16:
		case TOK_TYPE_S32:
		case TOK_TYPE_S64:
		case TOK_TYPE_U8:
		case TOK_TYPE_U16:
		case TOK_TYPE_U32:
		case TOK_TYPE_U64:
			break;

		default:
			break;
	}

	return token;
}


static Token* tokenize(LexerContext* context)
{
	next_token(context);
	save_token_start(context);

	StringCapture* capture;
	Token* token = NULL;

	switch (context->lookahead) {
		case ' ':
            adjust_position(context, 1);
            return FAKE_TOKEN;
		case '\t':
            adjust_position(context, 4);
            return FAKE_TOKEN;
		case '\n':
			adjust_position(context, WHITESPACE);
            return FAKE_TOKEN;
		case EOF:
			return NULL;

		default: {
			if (isalpha(context->lookahead) || context->lookahead == '_') {
				capture = capture_string(context);

				token =
					// Types
					strstr(capture->str, "u8")  
						? create_token(context, TOK_TYPE_U8,  NULL, capture->len) :
					strstr(capture->str, "u16") 
						? create_token(context, TOK_TYPE_U16, NULL, capture->len) :
					strstr(capture->str, "u32") 
						? create_token(context, TOK_TYPE_U32, NULL, capture->len) :
					strstr(capture->str, "u64") 
						? create_token(context, TOK_TYPE_U64, NULL, capture->len) :
					strstr(capture->str, "s8")  
						? create_token(context, TOK_TYPE_S8,  NULL, capture->len) :
					strstr(capture->str, "s16") 
						? create_token(context, TOK_TYPE_S16, NULL, capture->len) :
					strstr(capture->str, "s32") 
						? create_token(context, TOK_TYPE_S32, NULL, capture->len) :
					strstr(capture->str, "s64") 
						? create_token(context, TOK_TYPE_S64, NULL, capture->len) :
					// OTHERWISE - Identifier
						create_token(context, TOK_IDENT, strdup(capture->str), capture->len);
			} else if (isdigit(context->lookahead)) {
				/* TODO - Only does positive integers */

				capture = capture_string(context);
				uint64_t* value = malloc(sizeof(uint64_t));
				*value = (uint64_t) strtoll(capture->str, 0, 0);
				token = create_token(context, TOK_INTEGER_LITERAL, value, capture->len);
			} else {
				log_lexer_error(context, "unknown grammar\n");
			}

			break;
		}
	}

	adjust_position(context, capture->len);
	destroy_string_capture(capture);
	
	return token;
}

void lex(LexerContext* context)
{
	Token* token;

	while ((token = tokenize(context))) {
        if (token == FAKE_TOKEN)
            continue;
        else
            fifo_push(context->tokens, token);
	}
}
