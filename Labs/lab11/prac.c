#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int count=0;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg){
    pthread_mutex_lock(&lock);
while (count<5){
    
    count++;
}
pthread_cond_signal(&cond);
pthrad_mutex_unlock(&lock);


}

void *thread2(void *arg){
pthread_mutex(&lock);
if (count<5){
    pthread_cond_wait(cond,lock);
}

}