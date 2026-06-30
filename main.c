#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void tokenize(char arr[], char *args[]);
int exit_shell(char *args[]);
int change_directory(char *args[]);
int print_working_directory(char *args[]);
int select_builtin(char *args[]);

int main()
{
    char *buff = NULL;
    size_t size = 0;
    ssize_t read;

    while(1)
    {
        printf("myshell> ");
        fflush(stdout);

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

        if (select_builtin(array))
        {
            continue;
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

int exit_shell(char *args[])
{
    if(args[1] != NULL)
    {
        printf("exit does not take any arguments\n");
        return 0;
    }
    else
    {
        exit(0);
    }
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

int change_directory(char *args[])
{
    char *home_dir = getenv("HOME");
    if (home_dir == NULL)
    {
        printf("HOME environment variable is not set\n");
        return 0;
    }

    if(args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        if(chdir(home_dir) != 0)
        {
            perror("chdir failed");
            return 0;
        }
        
        return 1;
    }
    else if(chdir(args[1]) != 0)
    {
        perror("chdir failed");
        return 0;
    }

    return 1;
}

int print_working_directory(char *args[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
        return 1;
    }
    else
    {
        perror("getcwd failed");
        return 0;
    }
}

int select_builtin(char *args[])
{
    typedef int (*function_ptr)(char *args[]);

    struct builtin_command
    {
        char *name;
        function_ptr func;
    };

    struct builtin_command commands[] = {
        {"exit", exit_shell},
        {"cd", change_directory},
        {"pwd", print_working_directory},
        {NULL, NULL}
    };

    for (int i = 0; commands[i].name != NULL; i++)
    {
        if (args[0] != NULL && strcmp(args[0], commands[i].name) == 0)
        {
            commands[i].func(args);
            return 1;
        }
    }

    return 0;
}