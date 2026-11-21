#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int status = EXIT_SUCCESS;

    while (1)
    {
        printf("reun> ");
        fflush(stdout);

        nread = getline(&line, &len, stdin);

        if (nread == -1)
        {
            if (ferror(stdin))
            {
                perror("getline");
                status = EXIT_FAILURE;
                break;
            }

            if (feof(stdin))
            {
                break;
            }

            status = EXIT_FAILURE;
            break;
        }

        // Eventually, we'll process commands here
    }

    free(line);

    return status;
}
