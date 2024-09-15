
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 14
#define SIZE 10

int buffer[SIZE];
int counter[10];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int i = *((int *)arg);
    int value = i;

    for (int j = 0; j < 1000; j++) {
        pthread_mutex_lock(&mutex);
        while ((buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7] + buffer[8] + buffer[9]) == SIZE) {
            pthread_cond_wait(&full, &mutex);
        }
        buffer[value]++;
        printf("Producer %d added a value of %d. Current count is %d\n", i, value, buffer[value]);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *consumer(void *arg) {
    int i = *((int *)arg);

    for (int j = 0; j < 1000; j++) {
        pthread_mutex_lock(&mutex);
        while (buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7] + buffer[8] + buffer[9] == 0) {
            pthread_cond_wait(&empty, &mutex);
        }
        int value;
        for (int k = 0; k < 10; k++) {
            if (buffer[k] > 0) {
                value = k;
                buffer[k]--;
                break;
            }
        }
        counter[value]++;
        printf("Consumer %d consumed a value of %d. Current count is %d\n", i, value, counter[value]);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t threads[NTHREADS];

    for (int i = 0; i < 10; i++) {
        buffer[i] = 0;
        counter[i] = 0;
    }

    for (int i = 0; i < 10; i++) {
        if (pthread_create(&threads[i], NULL, producer, (void *)&i)) {
            printf("Error creating producer thread\n");
            return 1;
        }
    }

    for (int i = 10; i < NTHREADS; i++) {
        if (pthread_create(&threads[i], NULL, consumer, (void *)&i)) {
            printf("Error creating consumer thread\n");
            return 1;
        }
    }

    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("Error joining thread\n");
            return 2;
        }
    }

    return 0;
}