#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define NUMTHREADS 4

void* tfunc (void *arg) {
    //sleep(1);
    printf("hello world from thread no. %d\n", gettid());
}

int main(int argc, char* argv[]){
    printf("hellllooooooooooooooooooooooo\n");
    // int numthreads = atoi(argv[1]);
    printf("main going to create %d therds\n", NUMTHREADS);

    pthread_t tids[NUMTHREADS];


    for(int i=0; i<NUMTHREADS; i++) {
        pthread_create( &tids[i],   // pointer where it'll store thread id
                        NULL, // phread_attr_t pointer
                        tfunc, // thread function pointer
                        NULL);
    }


    for(int i=0; i<NUMTHREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}

