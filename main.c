#include <stdio.h>
#include <string.h>

void tokenize(char arr[], char *args[]);

int main()
{
    char buff[100];

    while(1)
    {
        printf("myshell> ");
        fflush(stdout);

        //when fget runs and the user types something, the input stream attaches a \n at the end
        //a C string is just an array of characters that has a \0 at the end
        if(fgets(buff, sizeof(buff), stdin) == NULL)
        {
            break;
        }

        buff[strcspn(buff, "\n")] = '\0';

        if(strcmp(buff, "exit") == 0)
        {
            break;
        }

        char *array[100];
        tokenize(buff, array);

        for(int i = 0; array[i] != NULL; i++)
        {
            printf("arg[%d] = %s\n", i, array[i]);
        }

    }

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
