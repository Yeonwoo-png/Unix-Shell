#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void tokenize(char arr[], char *args[]);

int main()
{
    char *buff = NULL;
    size_t size = 0;
    ssize_t read;

    while(1)
    {
        printf("myshell> ");
        fflush(stdout);

        //when fget runs and the user types something, the input stream attaches a \n at the end
        //a C string is just an array of characters that has a \0 at the end
        read = getline(&buff, &size, stdin);
        if(read == -1)
        {
            break;
        }

        buff[strcspn(buff, "\n")] = '\0';

        if(buff[0] == '\0')
        {
            continue;
        }

        char *array[100];
        tokenize(buff, array);

        if(array[0] != NULL && strcmp(array[0], "exit") == 0)
        {
            break;
        }

        pid_t p = fork();
        if (p == 0)
        {
            execvp(array[0], array);

            perror("execvp failed");
            exit(1);
        }
        else if (p < 0)
        {
            perror("fork failed");
        }
        else
        {
            waitpid(p, NULL, 0);
        }
    }

    free(buff);

    return 0;
}

void tokenize(char arr[], char *args[])
{
    char *token;
    int i = 0;

    token = strtok(arr, " ");

    while(token != NULL)
    {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    args[i] = NULL;

    return;
}
