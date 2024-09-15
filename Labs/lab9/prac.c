
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

#define ARR_SIZE 100000000
#define N_THREADS 4 

int sum(int arr[], int size){
    int sum=0;
    for(int i=0;i<size;i++){
        sum=sum+arr[i];
    }
    return sum;
}
typedef struct targs{
    int *arr;
    int size;
}targs;

void *func(void *arg){
    targs * targ= (targs *) arg;
    int localsum=0;
    localsum=sum(targ->arr,targ->size);
    int *ret=malloc(sizeof(int)*1);
    *ret=localsum;
    return (void *)ret;
}
int main()
{

    int arr[ARR_SIZE];
    for (int i = 0; i < ARR_SIZE; i++)
    {
        arr[i]=1;
    }
    targs targs[N_THREADS];

    pthread_t tids[N_THREADS];
    for(int i=0;i<N_THREADS;i++){
        targs[i].arr=arr+(i*(ARR_SIZE/N_THREADS));
        targs[i].size=ARR_SIZE/N_THREADS;
    
        pthread_create(&tids[i],
        NULL,
        func,
        &targs[i]);
    }
    
    int *tret=NULL;
    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_join(tids[i],(void **)&tret);
        printf("%d ",*tret);
    }
    free(tret);
}