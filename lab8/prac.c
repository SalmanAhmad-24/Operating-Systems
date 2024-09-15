

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define COMMAND_SIZE 100
#define ARG_SIZE 10

int main()
{
    char command[COMMAND_SIZE];
    char *args[ARG_SIZE];
    while (1)
    {
        printf("Enter a command \n");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';
        if (strcmp(command, "exit") == 0)
        {
            printf("Bye Bye\n");
            break;
            printf("hellllll");
        }
       
            pid_t pid;
            int i = 0;
            char *token = strtok(command, " ");
            while (token != NULL && i < ARG_SIZE - 1)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;
            pid = fork();
            if (pid < 0)
            {
                perror("Failed to create child \n");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                execvp(args[0], args);
            }
            else
            {
                wait(NULL);
                printf("Child process terminated\n");
                printf("parent process terminated\n");
            }
        
    }
    return 0;
}