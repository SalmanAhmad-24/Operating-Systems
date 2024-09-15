#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
void *calcAverage(void * args){
   int *p=(int *) args;
   int sum=0;
   for(int i=0;i<5;i++){
    sum=sum+ *(p+i);
   }
   int average=sum/5;
   int *ret=malloc(sizeof(int)*1);
   *ret=average;
   return (void *) ret;
}


void *calcMax(void *args){
    int *p=(int *)args;
    int max=*p;
    for (int i=1;i<5;i++){
        if(max> *(p+i)){
            max=max;
        }
        else{
            max=*(p+i);
        }
    }
    int *ret=malloc(sizeof(int)*1);
    *ret=max;
    return (void*)ret;
}

int main(int argc ,char *argv[])
{
    printf("Enter 5 numbers \n");
    int a[5];
    for(int i=0;i<5 ;i++){
        scanf("%d",&a[i]);
    }

    pthread_t average,max;
    pthread_create(&average,NULL,calcAverage,&a);
    pthread_create(&max,NULL,calcMax,&a);
    int *tret=NULL;
    int *tret2=NULL;
    pthread_join(average,(void **)&tret);
    pthread_join(max,(void **)&tret2);
    printf("avergae is : %d\n",*tret);
    printf("max is : %d\n",*tret2);
    free(tret);
    free(tret2);
    return 0;
}