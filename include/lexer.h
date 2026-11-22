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

void token_list_destroy(TokenList *tokens);

#endif
