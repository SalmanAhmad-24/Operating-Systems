#include<stdlib.h>
#include<stdio.h>

typedef struct proc {
    char* name;
    int id;
    int runtime;
    int arrtime;
    struct proc* next;
} proc;

proc* sortedInsert(proc* head, proc* p) {
    if (head == NULL) {
        head = p;
        return head;
    }
    if (head->runtime > p->runtime) {
        p->next = head;
        head = p;
        return head;
    }
    proc* prev = head;
    proc* temp;
    if (head->next != NULL) {
        temp = head->next;
    } else {
        temp = NULL;
    }
    while (temp != NULL) {
        if (temp->runtime > p->runtime) {
            prev->next = p;
            p->next = temp;
            return head;
        }
        if (temp->next == NULL) {
            temp->next = p;
            return head;
        }
        temp = temp->next;
        prev = prev->next;
    }
    temp->next = p;
    return head;
}

proc* insert(proc* head, proc* p) {
    if (head == NULL) {
        head = p;
        return head;
    }
    proc* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = p;
    return head;
}

void printStatus(int time, proc* cpu, proc* readyQueue) {
    printf("%d:", time);
    if (cpu != NULL) {
        printf("%s:", cpu->name);
    } else {
        printf("idle:");
    }
    if (readyQueue == NULL) {
        printf("empty:\n");
    } else {
        proc* temp = readyQueue;
        while (temp != NULL) {
            printf("%s(%d),", temp->name, temp->runtime);
            temp = temp->next;
        }
        printf(":\n");
    }
}
