#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "utilities.h"
#include "u2021562_fifo.h"
#include "u2021562_sjf.h"
#include "u2021562_stcf.h"
#include "u2021562_rr.h"

// Function prototypes
proc* insert(proc*, proc*);
void print_process_list(proc*);
void print_single_process(proc*);
proc* read_process_from_buffer(char*);

void print_process_list(proc* head) {
	proc* current = head;
	while (current != NULL) {
		print_single_process(current);
		current = current->next;
	}
}

void print_single_process(proc* process) {
	printf("Process Name: %s ID: %d Duration: %d Arrival Time: %d\n", process->name, process->id, process->runtime, process->arrtime);
}

proc* read_process_from_buffer(char* buffer) {
	proc* temp = malloc(sizeof(proc));
	temp->next = NULL;
	char* token;
	int count = 0;

	token = strtok(buffer, ":");
	while (token != NULL) {
		if (count == 0) {
			temp->name = strdup(token);
		} else if (count == 1) {
			temp->id = atoi(token);
		} else if (count == 2) {
			temp->runtime = atoi(token);
		}
		char delim = ':';
		if (count >= 3) {
			temp->arrtime = atoi(token);
			delim = '\n';
		}
		token = strtok(NULL, &delim);
		count++;
	}
	return temp;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Incorrect number of arguments. The first argument should be the path to the .dat file, and the second should be the policy.\n");
		exit(-1);
	}

	char* filepath = argv[1];
	char* policy = argv[2];

	FILE* file = fopen(filepath, "r");
	char buffer[1000];

	proc* process_list = NULL;

	proc* p;
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (buffer[0] != '#') {
			p = read_process_from_buffer(buffer);
			process_list = insert(process_list, p);
		}
	}

	if (strcmp(policy, "FIFO") == 0) {
		FIFO(process_list);
	} else if (strcmp(policy, "SJF") == 0) {
		shortestJobFirstScheduling(process_list);
	} else if (strcmp(policy, "STCF") == 0) {
		shortestTimeToCompletionFirstScheduling(process_list);
	} else if (strcmp(policy, "RR") == 0) {
		roundRobinScheduling(process_list);
	} else {
		printf("Invalid policy argument. Accepted arguments: 'FIFO', 'SJF', 'STCF', 'RR'\n");
		exit(-1);
	}
}
