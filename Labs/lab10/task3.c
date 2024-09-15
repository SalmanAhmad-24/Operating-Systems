#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Node structure for the linked list
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Global variables
Node* head = NULL;
pthread_mutex_t lock;

// Function to insert an element at the beginning of the linked list
void insertElement(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = head;
    head = newNode;
}
pthread_mutex_t lock;
// Function to count the number of elements in the linked list
int countElements() {
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Function to be executed by each thread (without synchronization)
void *insertElementsWithoutSync(void *arg) {
    int i;
    int threadID = *((int *)arg);
    for (i = 0; i < 10000; ++i) {
        insertElement(i + threadID * 10000); // Insert unique elements for each thread
    }
    return NULL;
}

// Function to be executed by each thread (with synchronization)
void *insertElementsWithSync(void *arg) {
    int i;
    int threadID = *((int *)arg);
    for (i = 0; i < 10000; ++i) {
        pthread_mutex_lock(&lock); // Lock mutex before accessing the shared linked list
        insertElement(i + threadID * 10000); // Insert unique elements for each thread
        pthread_mutex_unlock(&lock); // Unlock mutex after accessing the shared linked list
    }
    return NULL;
}

int main() {
    int i;
    pthread_t threads[4];
    int threadIDs[4];

    // Initialize the mutex lock
    pthread_mutex_init(&lock, NULL);

    // Create 4 threads without synchronization
    for (i = 0; i < 4; ++i) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, insertElementsWithoutSync, &threadIDs[i]);
    }

    // Wait for threads to finish
    for (i = 0; i < 4; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Print the total number of elements without synchronization
    printf("Total elements without synchronization: %d\n", countElements());

    // Reset the linked list for synchronized insertion
    head = NULL;

    // Create 4 threads with synchronization
    for (i = 0; i < 4; ++i) {
        pthread_create(&threads[i], NULL, insertElementsWithSync, &threadIDs[i]);
    }

    // Wait for threads to finish
    for (i = 0; i < 4; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Print the total number of elements with synchronization
    printf("Total elements with synchronization: %d\n", countElements());

    // Destroy the mutex lock
    pthread_mutex_destroy(&lock);

    return 0;
}

