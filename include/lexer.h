#ifndef REUN_LEXER_H
#define REUN_LEXER_H

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE
} TokenType;

typedef struct {
    TokenType type;
    char *text;
} Token;

#endif
