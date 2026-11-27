#ifndef REUN_LEXER_H
#define REUN_LEXER_H

#include <stddef.h>

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND
} TokenType;

typedef struct {
    TokenType type;
    char *text;
} Token;

typedef struct {
    Token *data;
    size_t size;
    size_t capacity;
} TokenList;

typedef enum {
    LEX_OK,
    LEX_ERR_MEMORY,
    LEX_ERR_UNTERMINATED_QUOTE,
    LEX_ERR_INCOMPLETE_ESCAPE
} LexStatus;

LexStatus lexer_tokenize(const char *input, TokenList *out);
void token_list_destroy(TokenList *tokens);

#endif
