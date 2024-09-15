
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PRODUCERS 10
#define NUM_CONSUMERS 4
#define BUFFER_SIZE 10

sem_t mutex, empty, full;
int buffer[BUFFER_SIZE];
int counter[10];
int allcount = 0;

void* producer(void* param) {
    int value = *((int*)param);
    for (int i = 0; i < 1000; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[value]++;
        printf("Producer %d added value %d\n", value, value);
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* consumer(void* param) {
    while (allcount < 10000) {
        sem_wait(&full);
        sem_wait(&mutex);
        int value = buffer[rand() % BUFFER_SIZE];
        counter[value]++;
        allcount++;
        printf("Consumer %ld read value %d\n", (long)param, value);
        sem_post(&mutex);
        sem_post(&empty);
        if (allcount == 10000) break;
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_t producers[NUM_PRODUCERS];
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, &i);
    }

    pthread_t consumers[NUM_CONSUMERS];
    for (long i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void*)i);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    for (long i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("All count: %d\n", allcount);
    for (int i = 0; i < 10; i++) {
        printf("Value %d count: %d\n", i, counter[i]);
    }

    return 0;
}