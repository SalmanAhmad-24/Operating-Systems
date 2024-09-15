#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* thread_function_1(void* arg) {
    pthread_mutex_lock(&lock);

    for(int i = 1; i <= 5; i++) {
        counter++;
        //usleep(1000);
        printf("Thread 1: counter = %d\n", counter);
    }

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    pthread_mutex_lock(&lock);
    while(counter < 10) {
        pthread_cond_wait(&cond, &lock);
    }

    for(int i = 11; i <= 15; i++) {
        counter++;
        printf("Thread 1: counter = %d\n", counter);
    }

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    return NULL;
}

void* thread_function_2(void* arg) {
    pthread_mutex_lock(&lock);
    while(counter < 5) {
        pthread_cond_wait(&cond, &lock);
    }

    for(int i = 6; i <= 10; i++) {
        counter++;
        //usleep(1000);
        printf("Thread 2: counter = %d\n", counter);
    }

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    pthread_mutex_lock(&lock);
    while(counter < 15) {
        pthread_cond_wait(&cond, &lock);
    }

    for(int i = 16; i <= 20; i++) {
        counter++;
        printf("Thread 2: counter = %d\n", counter);
    }

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    if(pthread_create(&thread1, NULL, thread_function_1, NULL)) {
        printf("Error creating thread 1\n");
        return 1;
    }

    if(pthread_create(&thread2, NULL, thread_function_2, NULL)) {
        printf("Error creating thread 2\n");
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final counter value: %d\n", counter);

    return 0;
}
