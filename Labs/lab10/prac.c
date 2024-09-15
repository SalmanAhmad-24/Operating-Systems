#include<stdio.h>
#include<pthread.h>
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER ;

int count=10;
void *counter(void * arg){
    if(count<20){
    pthread_mutex_lock(&mutex);
    printf("Thread value = %d\n thread_id %ld",count, (long)arg);
        count++;
    pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t tids[10];
    long i;
    for( i=0;i<10;i++){
        pthread_create(&tids[i],
        NULL,
        counter,
        (void *)&i);
    }
    // Waiting for all threads to finish
    for( i=0;i<10;i++){
        pthread_join(tids[i],NULL);
    }
}