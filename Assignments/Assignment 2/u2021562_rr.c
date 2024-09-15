#include<stdlib.h>
#include<stdio.h>
#include "utilities.h"

void roundRobinScheduling(proc* processList) {
	int currentTime = 1;
	proc* processHead = processList;
	proc* readyQueue = NULL;
	proc* currentProcess = NULL;

	while (processHead != NULL || readyQueue != NULL || currentProcess != NULL) {
		printStatus(currentTime, currentProcess, readyQueue);
		
		if (currentProcess != NULL) {
			currentProcess->runtime = currentProcess->runtime - 1;
			if (currentProcess->runtime <= 0) {
				if (readyQueue != NULL) {
					currentProcess = readyQueue;
					readyQueue = readyQueue->next;
				} else {
					currentProcess = NULL;
				}
			} else {
				if (readyQueue != NULL) {
					proc* temp = currentProcess;
					temp->next = NULL;
					currentProcess = readyQueue;
					readyQueue = readyQueue->next;
					readyQueue = insert(readyQueue, temp);
				}
			}
		} 

		proc* temp = processHead;
		while (temp != NULL && temp->arrtime == currentTime) {
			processHead = temp->next;
			temp->next = NULL;
			readyQueue = insert(readyQueue, temp);
			temp = processHead;
		}
		
		proc* prev = processHead;
		if (processHead != NULL) {
			temp = processHead->next;
		} else {
			temp = NULL;
		}

		while (temp != NULL) {
			if (temp->arrtime == currentTime) {
				prev->next = temp->next;
				temp->next = NULL;
				readyQueue = insert(readyQueue, temp);
				temp = prev->next;
			}
			else {
				temp = temp->next;
				prev = prev->next;
			}
		}

		if (currentProcess == NULL && readyQueue != NULL) {
			currentProcess = readyQueue;
			readyQueue = readyQueue->next;
		}
		currentTime++;	
	}
}
