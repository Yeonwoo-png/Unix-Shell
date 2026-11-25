#include "lexer.h"

#include <ctype.h>
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

static bool finish_word(WordBuffer *word, TokenList *tokens, bool *word_started)
{
    if (!*word_started) {
        return true;
    }

    if (word->data == NULL)
    {
        char *temp = malloc(1);

        if (temp == NULL) {
            return false;
        }

        temp[0] = '\0';
        word->data = temp;
        word->capacity = 1;
    }

    Token token = {
        .type = TOKEN_WORD,
        .text = word->data
    };

    if (!token_list_push(tokens, token)) {
        return false;
    }

    word->data = NULL;
    word->size = 0;
    word->capacity = 0;
    *word_started = false;

    return true;
}

LexStatus lexer_tokenize(const char *input, TokenList *out)
{
    WordBuffer word = {0};
    LexerState state = LEX_STATE_NORMAL;
    LexStatus status = LEX_ERR_MEMORY;
    bool word_started = false;
    size_t i = 0;

    *out = (TokenList){0};

    while (input[i] != '\0')
    {
        char c = input[i];

        switch (state)
        {
            case LEX_STATE_NORMAL:
                if (c == '"')
                {
                    state = LEX_STATE_DOUBLE_QUOTE;
                    word_started = true;
                }
                else if (c == '\'')
                {
                    state = LEX_STATE_SINGLE_QUOTE;
                    word_started = true;
                }
                else if (isspace((unsigned char)c))
                {
                    if (!finish_word(&word, out, &word_started)) {
                        goto cleanup;
                    }
                }
                else if (c == '|')
                {
                    if (!finish_word(&word, out, &word_started)) {
                        goto cleanup;
                    }

                    Token token = {
                        .type = TOKEN_PIPE,
                        .text = NULL
                    };

                    if (!token_list_push(out, token)) {
                        goto cleanup;
                    }
                }
                else
                {
                    word_started = true;

                    if (!word_buffer_push(&word, c)) {
                        goto cleanup;
                    }
                }
                break;

            case LEX_STATE_SINGLE_QUOTE:
                if (c == '\'')
                {
                    state = LEX_STATE_NORMAL;
                }
                else
                {
                    if (!word_buffer_push(&word, c)) {
                        goto cleanup;
                    }
                }
                break;

            case LEX_STATE_DOUBLE_QUOTE:
                if (c == '"')
                {
                    state = LEX_STATE_NORMAL;
                }
                else
                {
                    if (!word_buffer_push(&word, c)) {
                        goto cleanup;
                    }
                }
                break;
        }

        i++;
    }

    if (state != LEX_STATE_NORMAL)
    {
        status = LEX_ERR_UNTERMINATED_QUOTE;
        goto cleanup;
    }

    if (!finish_word(&word, out, &word_started)) {
        goto cleanup;
    }

    return LEX_OK;

cleanup:
    free(word.data);
    token_list_destroy(out);
    return status;
}
