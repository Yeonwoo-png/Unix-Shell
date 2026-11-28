#define _POSIX_C_SOURCE 200809L

#include "parser.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool command_reserve_argv(Command *command, size_t new_capacity)
{
    if (new_capacity <= command->argv_capacity) {
        return true;
    }

    size_t max_capacity = SIZE_MAX / sizeof(*command->argv);

    if (new_capacity > max_capacity) {
        return false;
    }

    char **temp = realloc(
        command->argv,
        new_capacity * sizeof(*command->argv)
    );

    if (temp == NULL) {
        return false;
    }

    command->argv = temp;
    command->argv_capacity = new_capacity;

    return true;
}

static bool command_push_arg(Command *command, const char *arg)
{
    size_t max_capacity = SIZE_MAX / sizeof(*command->argv);

    if (command->argc > max_capacity - 2) {
        return false;
    }

    size_t required_capacity = command->argc + 2;

    if (required_capacity > command->argv_capacity)
    {
        if (!command_reserve_argv(command, required_capacity)) {
            return false;
        }
    }

    char *copy = strdup(arg);

    if (copy == NULL) {
        return false;
    }

    command->argv[command->argc] = copy;
    command->argc++;
    command->argv[command->argc] = NULL;

    return true;
}
