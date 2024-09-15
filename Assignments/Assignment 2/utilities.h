#ifndef UTILS_H
#define UTILS_H

typedef struct proc {
    char* name;
    int id;
    int runtime;
    int arrtime;
    struct proc* next;
} proc;

proc* sortedInsert(proc* head, proc* p);
proc* insert(proc* head, proc* p);
void printStatus(int time, proc* cpu, proc* readyqueue);


#endif
