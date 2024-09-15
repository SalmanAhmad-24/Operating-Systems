#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid1,pid2;
    pid1=fork();

    if (pid1<0){
        perror("Fork failed \n");
        exit(EXIT_FAILURE);
    }
    else if (pid1==0){
        execlp("ls","ls","-l",NULL);
        printf("Exec unable to succeed");
        exit(EXIT_FAILURE);
    }
    else{
        pid2=fork();
        if (pid2<0){
        perror("Fork failed \n");
        exit(EXIT_FAILURE);
    }
    else if (pid2==0){
        execlp("ps","ps","aux",NULL);
        printf("Exec unable to succeed");
        exit(EXIT_FAILURE);
    }
    else{
        int status;
        waitpid(pid1,&status,0);
        printf("First child executed \n");
        waitpid(pid2,&status,0);
        printf("Second child executed \n");

        printf("Parent process compeleted \n");
    }

    }



}