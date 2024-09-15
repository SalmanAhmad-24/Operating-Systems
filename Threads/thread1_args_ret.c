#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define NUMTHREADS 4    // number of threads to create
#define N 1000          // array size

// struct for passing argumetns to each thread
typedef struct targ{
    int *start; // start of array for each thread
    int size;   // num elements to process by each thread
} targ;

int sum(int a[], int size) {
    int sum = 0;
    for(int i=0; i<size; i++)
        sum = sum + a[i];

    return sum;
}


// thread function
void* tfunc (void *arg) {
    targ *p = (targ *)arg;  // cast the argumetn
    int localsum = sum(p->start, p->size);  // get sum of its part

    int *ret = malloc(sizeof(int)*1);   // allocate mem for sum in heap
    *ret = localsum;
    return (void *) ret;
}

int main(int argc, char* argv[]){

    int array[N];   // declare array
    // initialize array
    for(int i=0; i<N; i++)
        array[i] = 1;

    // perform a sum serailly in the main thread
    printf("main sum is %d\n", sum(array, N));

    

    printf("main going to create %d therds\n", NUMTHREADS);

    pthread_t tids[NUMTHREADS];
    targ  targs[NUMTHREADS];

    // creat threads
    for(int i=0; i<NUMTHREADS; i++) {
        // intialize arguments for thread to be created
        targs[i].start = array + (i*N/NUMTHREADS);
        targs[i].size = N/NUMTHREADS;


        pthread_create( &tids[i],   // pointer where it'll store thread id
                        NULL, // phread_attr_t pointer
                        tfunc, // thread function pointer
                        &targs[i]);
    }

    int *tret = NULL;
    int finalsum = 0;
    for(int i=0; i<NUMTHREADS; i++) {   
        // wait for thread to finish and retrieve its return valu
        pthread_join(tids[i], (void **)&tret);//  is then a typecast, telling the compiler to treat the address of tret as a pointer to a void pointer.
        printf("Thread %lu returned %d\n", tids[i], *tret);
        finalsum += *tret;  // cumulate partial sum returned by each thread

        free(tret); // free the memory allocated by thread on heap
    }
    printf("Final sum = %d\n", finalsum);   // total sum

    return 0;
}
