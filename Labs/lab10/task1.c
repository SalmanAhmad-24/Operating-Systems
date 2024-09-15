// // Task1
// #include <stdio.h>
// #include <pthread.h>

// #define NUM_THREADS 10

// int count = 10;

// void *counter(void *thread_id) {
//     if (count <= 20) {
//         printf("Thread %ld: %d\n", (long)thread_id, count);
//         count++;
//     }
//     pthread_exit(NULL);
// }

// int main() {
//     pthread_t threads[NUM_THREADS];
//     long i;

//     for (i = 0; i < NUM_THREADS; i++) {
//         pthread_create(&threads[i], NULL, counter, (void *)i);
//     }

//     for (i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     return 0;
// }

#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10

int count = 10;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *counter(void *thread_id) {
    
        if (count <= 20) {
        pthread_mutex_lock(&mutex);
            printf("Thread %ld: %d\n", (long)thread_id, count);
            count++;
            pthread_mutex_unlock(&mutex);
        } else {
            //pthread_mutex_unlock(&mutex);
        
        }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    long i;

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, counter, (void *)i);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

