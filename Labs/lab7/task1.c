#include <stdio.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <unistd.h>

void calculator(float a, float b)
{
    printf("a + b : %f\n", a + b);
    printf("a - b : %f\n", a - b);
    printf("a * b : %f\n", a * b);
    if(b != 0)
        printf("a / b : %f\n", a / b);
}

int main()
{
    int pid; 
    float num1, num2;
    printf("enter two numbers\n");
    scanf("%f", &num1);
    scanf("%f", &num2);
   
    pid = fork();
    if(pid < 0)
        printf("error\n");
    else if (pid == 0){ //child process
        calculator(num1, num2);
    }
    else{ //parent process
       
       wait(NULL);
       printf("calculation is done\n");
    }
    

    return 0;
}
