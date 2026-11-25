#ifndef REUN_LEXER_H
#define REUN_LEXER_H

#include <stddef.h>

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE
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
    LEX_ERR_UNTERMINATED_QUOTE
} LexStatus;

LexStatus lexer_tokenize(const char *input, TokenList *out);
void token_list_destroy(TokenList *tokens);

#endif
