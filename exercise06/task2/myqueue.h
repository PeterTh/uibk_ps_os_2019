// This header file provides a basic queue implementation


// ========== Forward Declarations ==========

// create and empty-initialize the queue
void create();

// returns 1 if queue is empty, 0 otherwise
int empty();

// returns the length of the queue
unsigned size();

// insert an element at the back of the queue
void push(unsigned value);

// get the first element from the front of the queue
unsigned front();

// remove the first element from the front of the queue
void pop();

// ========== Implementation Detail ==========

#include <sys/queue.h> // provides the internal queue implementation

unsigned myQueueLength = 0;

STAILQ_HEAD(stailhead, entry) head = STAILQ_HEAD_INITIALIZER(head);

struct stailhead *headp;

struct entry {
	unsigned content;
	STAILQ_ENTRY(entry) entries;
};

void create() {
	STAILQ_INIT(&head);
}

int empty() {
	return STAILQ_EMPTY(&head);
}

unsigned size() {
	return myQueueLength;
}

void push(unsigned value) {
	struct entry* myElement = malloc(sizeof(struct entry));
	myElement->content = value;
	STAILQ_INSERT_TAIL(&head, myElement, entries);
	myQueueLength++;
}

unsigned front() {
	struct entry* myElement = STAILQ_FIRST(&head);
	return myElement->content;
}

void pop() {
	struct entry* myElement = STAILQ_FIRST(&head);
	STAILQ_REMOVE_HEAD(&head, entries);
	free(myElement);
	myQueueLength--;
}

