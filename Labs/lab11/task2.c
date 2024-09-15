#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10

void *sum_fun(void *arg);
void *average_fun(void *arg);

pthread_barrier_t barrier;
int numbers[N];
double sum;
double average;

int main() {
    pthread_t thread1, thread2;

    for (int i = 0; i < N; i++) {
        printf("Enter number %d: ", i + 1);
        scanf("%d", &numbers[i]);
    }

    pthread_barrier_init(&barrier, NULL, 2);

    pthread_create(&thread1, NULL, sum_fun, NULL);
    pthread_create(&thread2, NULL, average_fun, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("The average of entered numbers is: %.2f\n", average);

    return 0;
}

void *sum_fun(void *arg) {
    sum = 0;
    for (int i = 0; i < N; i++) {
        sum += numbers[i];
    }
    pthread_barrier_wait(&barrier);
    return NULL;
}

void *average_fun(void *arg) {
    pthread_barrier_wait(&barrier);
    average = sum / N;
    return NULL;
}
