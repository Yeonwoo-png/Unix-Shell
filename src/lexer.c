#include "lexer.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    LEX_STATE_NORMAL,
    LEX_STATE_SINGLE_QUOTE,
    LEX_STATE_DOUBLE_QUOTE
} LexerState;

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} WordBuffer;

static bool word_buffer_push(WordBuffer *buffer, char c)
{
    if (buffer->size >= SIZE_MAX - 1) {
        return false;
    }

    if (buffer->size + 1 >= buffer->capacity) {
        size_t new_capacity;

        if (buffer->capacity == 0) {
            new_capacity = 16;
        }
        else {
            if (buffer->capacity > SIZE_MAX / 2) {
                return false;
            }

            new_capacity = buffer->capacity * 2;
        }

        char *temp = realloc(buffer->data, new_capacity);

        if (temp == NULL) {
            return false;
        }

        buffer->data = temp;
        buffer->capacity = new_capacity;
    }

    buffer->data[buffer->size] = c;
    buffer->size++;
    buffer->data[buffer->size] = '\0';

    return true;
}

static bool token_list_reserve(TokenList *tokens, size_t new_capacity)
{
    if (new_capacity <= tokens->capacity) {
        return true;
    }

    size_t max_capacity = SIZE_MAX / sizeof(Token);

    if (new_capacity > max_capacity) {
        return false;
    }

    Token *temp = realloc(tokens->data, new_capacity * sizeof(Token));

    if (temp == NULL) {
        return false;
    }

    tokens->data = temp;
    tokens->capacity = new_capacity;

    return true;
}

static bool token_list_push(TokenList *tokens, Token token)
{
    if (tokens->size == tokens->capacity)
    {
        size_t new_capacity;

        if (tokens->capacity == 0)
        {
            new_capacity = 4;
        }
        else
        {
            size_t max_capacity = SIZE_MAX / sizeof(Token);

            if (tokens->capacity > max_capacity / 2)
            {
                return false;
            }

            new_capacity = tokens->capacity * 2;
        }

        if (!token_list_reserve(tokens, new_capacity))
        {
            return false;
        }
    }

    tokens->data[tokens->size] = token;
    tokens->size++;

    return true;
}

void token_list_destroy(TokenList *tokens)
{
    if (tokens == NULL) {
        return;
    }

    for (size_t i = 0; i < tokens->size; i++) {
        free(tokens->data[i].text);
    }

    free(tokens->data);
    tokens->data = NULL;
    tokens->size = 0;
    tokens->capacity = 0;
}
