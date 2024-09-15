#include<stdlib.h>
#include<stdio.h>
#include "utilities.h"

void shortestTimeToCompletionFirstScheduling(proc* processList) {
	int currentTime = 1;
	proc* processHead = processList;
	proc* readyQueue = NULL;
	proc* currentProcess = NULL;

	while (processHead != NULL || readyQueue != NULL || currentProcess != NULL) {
		printStatus(currentTime, currentProcess, readyQueue);
		
		if (currentProcess != NULL) {
			if (readyQueue != NULL && readyQueue->runtime < currentProcess->runtime) {
				proc* temp = currentProcess;
				temp->next = NULL;
				currentProcess = readyQueue;
				readyQueue = readyQueue->next;
				readyQueue = sortedInsert(readyQueue, temp);
			}
			currentProcess->runtime = currentProcess->runtime - 1;
			if (currentProcess->runtime <= 0) {
				currentProcess = NULL;
			}
		} 

		proc* temp = processHead;
		while (temp != NULL && temp->arrtime == currentTime) {
			processHead = temp->next;
			temp->next = NULL;
			readyQueue = sortedInsert(readyQueue, temp);
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
				readyQueue = sortedInsert(readyQueue, temp);
				temp = prev->next;
			}
			else {
				temp = temp->next;
				prev = prev->next;
			}
		}

		if (currentProcess == NULL && readyQueue  != NULL) {
			currentProcess = readyQueue;
			readyQueue = readyQueue->next;
		}
		currentTime++;	
	}
}
