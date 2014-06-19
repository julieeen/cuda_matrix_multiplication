/*
 ============================================================================
 Name        : help.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : some functions 5 getting work done
 ============================================================================
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#include "help.h"
 
int isANumber(char value) {
	// ASCII : 0 = 48, 9 = 57
	if ((47 < value) && (value < 58)) {
		return 1;
	}
	return 0;
}

int isPartOfNumber(char value) {
	if ((47 < value) && (value < 58)) {	// ASCII : 0 = 48, 9 = 57
		return 1;
	}
	if (value == 46) {			// Char is a point
		return 1;
	}
	return 0;
}

/*
 ***************** Implements a small timestop function with milliseconds ***********
 */
double last_measured_time = 0.0;

struct timeval start,stop;
/*
 * start a clock
 * clock() falsified  the results by multithreading
 */
void run_timer() {
		last_measured_time = 0.0;
    gettimeofday(&start, NULL);
//	time(&start);
}

/*
 * reset timer and return bygone time
 */
double stop_timer() {
    gettimeofday(&stop, NULL);
    last_measured_time = difftime(stop.tv_sec, start.tv_sec) 
			+ (difftime(stop.tv_usec,start.tv_usec)/1000000);
		start = (struct timeval){0};
		stop = (struct timeval){0};
		return last_measured_time;
}

/*
 ============================================================================
 Name        : Stack.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : Stack implementation 4 parsing a matrix
 ============================================================================
 */


void init(node* head) {
	head = NULL;
}

/*
 * Stack as linked list, FILO
 */
node* push(node* head, float data) {
	node* tmp = (node*) malloc(sizeof(node));
	if (tmp == NULL ) {
		perror("Not possible to stack matrix elements!");
	}
	tmp->data = data;
	tmp->next = head;
	head = tmp;
	return head;
}

/*
 * Pop out last stacked element
 */
node* pop(node *head, float * element) {
	node* tmp = head;
	*element = head->data;
	head = head->next;
	free(tmp);
	return head;
}
