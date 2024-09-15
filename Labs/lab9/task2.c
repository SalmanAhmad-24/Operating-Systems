


#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

struct myThread{
    int quadrant;
};
typedef struct myThread myThread;

int mat[1000][1000];
int SUM=0;

void printSum(int sum){
    int n=2e7;
    while(n--);
    printf("SUM: %d\n", sum);
}

void *initializeSetSum(void *argue){
    myThread *args = (myThread *) argue;
    int q = args->quadrant;
    
    int x1,x2,y1,y2;
    if(q == 1){
        x1=0,y1=0;
        x2=500,y2=500;
    } else if(q == 2){
        x1=500,y1=0;
        x2=1000,y2=500;
    } else if(q == 3){
        x1=0,y1=500;
        x2=500,y2=1000;
    } else if(q == 4){
        x1=500,y1=500;
        x2=1000,y2=1000;
    } else{
        printf("Invalid quadrant\n");
        return NULL;
    }

    int sum=0;
    for(int i=x1; i<x2; i++){
        for(int j=y1; j<y2; j++){
            // mat->arr[i][j] = 1;
            mat[i][j] = 1;
            sum++;
        }
    }
    SUM += sum;
    return NULL;
}

void T2using4Threads(){
    pthread_t threads[4];
    myThread threadParameters[4];
    int threadErrors;

    for(int i=0; i<4; i++){
        threadParameters[i].quadrant = i+1;
        threadErrors = pthread_create(&threads[i], NULL, initializeSetSum, (void *)&threadParameters[i]);
        if(threadErrors){
            perror("Error creating thread\n");
            return;
        }
    }

    void *threadResult;
    int sum = 0;
    for(int i=0; i<4; i++){
        pthread_join(threads[i], NULL);\
    }

    printf("\nSUM: %d\n", SUM);
}

void T2using1Thread(){
    int sum=0;
    for(int i=0; i<1000; i++){
        for(int j=0; j<1000; j++){
            mat[i][j] = 1;
            sum++;
        }
    }
    printSum(sum);
}

int main(){
    
    clock_t start,end;
    double cpu_time_used;

    start = clock();
    T2using4Threads();
    end = clock();
    cpu_time_used = ((double) (end-start)/CLOCKS_PER_SEC);
    printf("CPU TIME used in 4 threads: %f\n\n", cpu_time_used);

    start = clock();
    T2using1Thread();
    end = clock();

    cpu_time_used = ((double) (end-start)/CLOCKS_PER_SEC);
    printf("CPU TIME used in 1 thread: %f\n", cpu_time_used);
}

