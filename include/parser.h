#ifndef REUN_PARSER_H
#define REUN_PARSER_H

#include <stddef.h>
#include "lexer.h"

typedef struct {
    TokenType type;
    char *filename;
} Redirection;

typedef struct {
    char **argv;
    size_t argc;
    size_t argv_capacity;

    Redirection *redirections;
    size_t redirection_count;
    size_t redirection_capacity;
} Command;

typedef struct {
    Command *commands;
    size_t count;
    size_t capacity;
} Pipeline;

#endif
