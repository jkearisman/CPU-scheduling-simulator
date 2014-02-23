#include "prog3.h"
#include "queueOps.h"


/*
clones the given queue, and returns the head of the new queue
allocates memory for a new element, with identical pid, arrival, and time to the corresponding element in the master queue,
then sets the next pointer in the list to whatever node is returned when this function is called recursively with the next element of the master list.
The terminating condition is a NULL list, in which case the program will return NULL
*/
Node *clone(Node *list) {
	if (list == NULL) 	//Terminating condition
		return NULL;
	Node *elt = makeATask(list -> pid, list -> arrival, list -> time);
	elt -> next = clone(list -> next); //Recursive call
	return elt; //Return the newly constructed node
}

/*
takes the given pid, arrival time, and process time
allocates memory to make a new task, and sets the element variables equal to the givens
afterwards, returns a pointer to the new struct
*/
Node* makeATask(int pid, int arrival, int time) {
        Node* task = malloc(sizeof (Node));
        if (task != NULL) { //If we have allocated memory successfully
		task -> pid = pid;
        	task -> arrival = arrival;
        	task -> time = time;
        	return task;
	} else {
		printf("No more memory to allocate, goodbye cruel world.");
		exit(-1);
	}
}


/*
A function used to determine how the eventlist is arranged when it is being set up. 
Earliest arriving tasks go first, and ties are broken by whichever has the shortest processing time.

This function follows simple rules to determine if n1 should be put before n2:

Returns 1 if the first given node has an earlier arrival than the second node

if the second node arrives first, it will return zero

if they arrive at the same time, we want the faster task to be processed first (shortest task first is optimal)
	so we return 1 if n1 is shorter than n2, and 0 otherwise
*/

int shouldGoFirst(Node *n1, Node *n2) {
	if (n1 -> arrival < n2 -> arrival)
		return	1;
	if (n2 -> arrival < n1 -> arrival)
		return 0;
	//If we get here, the arrivals are equal
	if (n1 -> time < n2 -> time)
		return 1;
	if (n2 -> time < n1 -> time)
		return 0;
	// Here, the tasks have the same arrival and same processing time, choosing n1 saves one run of the addInPlace loop, so that is what we choose
	return 1;

}

/*
adds the given event to the list
        each new process that arrives goes through a simple process
        1) If the list is empty, or the new process belongs at the beginning of the list,
                then it is put in front, and the process pointer is set to the old head of the list
        2) Otherwise, it looks through the list until it finds a process that arrives later
                then, it inserts itself in the list

*/
void addInPlace(Node **list, Node *newProc) {
        //This first part is for if the new event needs to go at the head of the list
        // we need to do some special things
        if (*list == NULL || shouldGoFirst(newProc, *list)) {
                newProc -> next = *list;
                *list = newProc;
                return;
        }
        // If it gets to this point, it means that it won't need to go at the head of the list
        Node *focus = *list;
        //Long while statement basically means while(not at end && we haven't found our place yet)
        while (focus -> next != NULL && !shouldGoFirst(newProc, focus-> next)) {
                focus = focus -> next; //advance down the list
        }
        //Setting up pointers to insert new element in the list
        newProc -> next = focus -> next;
        focus -> next = newProc;
        return; //All done
}


/*
for any given element, this function will remove the next element in the linked list 
then patch the gap by linking the given element pointer to the following element
afterwards, it frees the memory allocated to the front proccess.
*/
void wipeFrontProc(Node **list) {
        if (list != NULL) {
                Node* toDel = *list;
                *list = toDel -> next;
                free(toDel);
        }
        else {
                printf("Error, tried to clear a process ");
                printf("when there was none\n");
                printf("Program will recover, but this is bad\n");
        }

}

/*
prints the queue so far
basically, just keeps printing out elements from the event list until it reaches the null pointer.

Future improvements: Institute some kind of checking on each element to check for duplicate pids, or circular linkages
*/
void printQueue(Node *list) {
        int place = 0;
        Node *focus = list;
        while (focus != 0) {
                printf("position %d, pid=%d, arrival=%d, time=%d\n",place, focus -> pid, focus->arrival,focus->time);
                focus = focus -> next;
                place++;
        }
}

/*
puts the given new process at the end of the linked list given by the list element
*/
void addToBackOfQueue(Node **list, Node *newProc) {
        newProc -> next = 0; //next is zero because it will be last in list
	if (*list == NULL)
		*list = newProc;
	else {
        	Node *focus = *list;
        	while (focus -> next != 0) {
        	        focus = focus -> next;
        	}
        	focus -> next = newProc;
	}
}


/* 
pushes the first element in the queue to the back of the queue 
Used in the round robin processing when an event has used up its timeslice
*/
void pushFrontToBack(Node **list) {
        Node* focus = *list;
        *list = focus -> next;
        addToBackOfQueue(list, focus);
}

