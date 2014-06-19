/*
 ============================================================================
 Name        : help.h
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 ============================================================================
 */

int isANumber(char value);

int isPartOfNumber(char value);

void run_timer();

double stop_timer();

/*
 ============================================================================
 Name        : stack.h
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 ============================================================================
 */

typedef struct node {
	float data;
	struct node* next;
}node;

void init(node * head);

node * push(node* head, float data);

node * pop(node *head, float * element);

