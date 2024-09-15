#include<stdlib.h>
#include<stdio.h>
#include "utilities.h"

void FIFO(proc* proclist) {
	int time = 1;
	proc* head = proclist;
	proc* readyqueue = NULL;
	proc* cpu = NULL;

	while (head != NULL || readyqueue != NULL || cpu != NULL) {
		printStatus(time, cpu, readyqueue);
		
		// Handle processes inside of the CPU
		if (cpu != NULL) {
			cpu->runtime = cpu->runtime - 1;
			if (cpu->runtime <= 0) {
				cpu = NULL;
			}
		} 

		// If any new processes arrive at this time, add them to the ready queue
		proc* current_process = head;
		while (current_process != NULL && current_process->arrtime == time) {
			head = current_process->next;
			current_process->next = NULL;
			readyqueue = insert(readyqueue, current_process);
			current_process = head;
		}

		proc* previous_process = head;
		if (head != NULL) {
			current_process = head->next;
		} else {
			current_process = NULL;
	}

	while (current_process != NULL) {
		if (current_process->arrtime == time) {
			previous_process->next = current_process->next;
			current_process->next = NULL;
			readyqueue = insert(readyqueue, current_process);
			current_process = previous_process->next;
		}
		else {
			current_process = current_process->next;
			previous_process = previous_process->next;
		}
	}

	if (cpu == NULL && readyqueue  != NULL) {
		cpu = readyqueue;
		readyqueue = readyqueue->next;
	}
	time++;
}	
}
