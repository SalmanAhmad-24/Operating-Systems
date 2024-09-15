#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char* argv[]){
if(argc!=2){
    printf("Insert valid command line arguments");
    exit(1);
}
int n = atoi(argv[1]);
int factorial =1;
for(int i=0;i<n;i++){
    factorial =factorial*(n-i);
}
printf("Factorial %d \n", factorial);

return 0;

}