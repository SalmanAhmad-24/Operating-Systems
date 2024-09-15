#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>


int main()
{
int n;
char str[20];
printf("Enter a number \n");
scanf("%d ",&n);
sprintf(str,"%d",n);
// char *str[]={str,NULL};
char *args[] = { str, NULL};
pid_t rc;
rc=fork();
if (rc<0){
    perror("Fork failed");
    exit(1);
}
else if(rc==0){
    execvp("/home/salman/Desktop/oscodes/lab8/factorial",args);
    exit(1);
}
wait(NULL);
if(n%2==0){
    printf("The number was even \n");
}
else{
    printf("The number was odd \n");
}
return 0;
}