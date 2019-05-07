
#include <stdlib.h>
#include <stdio.h>

#include "myqueue.h" // provides a basic queue implementation

// print queue information
void printQueue() {
	printf("length: %u", size());
	if(!empty()) {
		printf(", front: %u", front());
	}
	printf("\n");
}

// remove elements from the queue if present
void consume() {
	if(!empty()) {
		pop();
	} else {
		printf("Sorry, queue is empty!\n");
	}
}

// add some elements to the queue
void produce() {
	push(0);
	push(42);
	push(37);
}

int main() {

	// create queue
	create();

	// just playing around a bit to see what happens

	printQueue();

	produce();

	printQueue();

	consume();

	printQueue();

	consume();
	consume();

	printQueue();

	consume();

	return EXIT_SUCCESS;
}
