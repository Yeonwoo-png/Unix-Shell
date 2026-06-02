#include <stdio.h>
#include <string.h>

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

        printf("%s\n",buff);

    }

    return 0;
}
