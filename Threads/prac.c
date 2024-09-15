#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define NUMTHREADS 4    // number of threads to create
#define N 1000          // array size


void *mythread(void *arg) {
long long int value = (long long int) arg;
printf("%lld\n", value);
return (void *) (value + 1);
}
int main(int argc, char *argv[]) {
pthread_t p;
long long int rvalue;
Pthread_create(&p, NULL, mythread, (void *) 100);
Pthread_join(p, (void **) &rvalue);
printf("returned %lld\n", rvalue);
return 0;
}