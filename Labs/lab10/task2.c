#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 12

int global_counter = 10;
pthread_t threads[NUM_THREADS];

void *counter(void *thread_id) {
    int local_counter = 0;
    
    while (local_counter < 4) {
        local_counter++;
        printf("Thread %ld: Local Counter: %d\n", (long)thread_id, local_counter);
    }

    // Critical section: Updating global counter
    global_counter += local_counter;
    
    pthread_exit(NULL);
}

int main() {
    long i;

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, counter, (void *)i);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Global Counter: %d\n", global_counter);

    return 0;
}
