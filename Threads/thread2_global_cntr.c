#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define NUMTHREADS 4

// global variable
static int counter = 0;

// thread function
void* tfunc (void *arg) {
    int count = *((int *)arg);
    // increment global var count times
    for(int i=0; i<count; i++)
        counter = counter + 1;

    return NULL;
}

int main(int argc, char* argv[]){

    // check for command line args
    if(argc<2) {
        printf("Usage: ./a.out count\n");
        return 0;
    }

    printf("main going to create %d therds\n", NUMTHREADS);

    // get count from command line
    int cnt = atoi(argv[1]);

    pthread_t tids[NUMTHREADS];

    // create threads
    for(int i=0; i<NUMTHREADS; i++) {

        pthread_create( &tids[i],   // pointer where it'll store thread id
                        NULL, // phread_attr_t pointer
                        tfunc, // thread function pointer
                        &cnt);
    }


    // wait for threads to finish
    for(int i=0; i<NUMTHREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("Final value of ocunter = %d\n", counter);

    return 0;
}
