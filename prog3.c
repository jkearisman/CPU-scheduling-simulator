#include "prog3.h"
#include "queueOps.h"
#include "process.h"

int main(int argc, char* argv[]) {
	// check to make sure we called the program with two args
	if (argc != 3) {
		printf("This program should be called with 2 arguments. See README for instructions\n");
		return -1;
	}
	
	//Save the number of input lines, and check to make sure it is at least 1
	int num = atoi(argv[1]);
	if (num < 1) {
		printf("We need to read in at least one line...\n");
		return -1;
	}

	//Check timeslice, should be at least 1
	int timeSlice = atoi(argv[2]);
	if (timeSlice < 1) {
		printf("Timeslice must be a positive number greater than 0. See README\n");
		return -1;
	}

	int pid, arrival, time;	//Variables to store what we read in from the file, until we make a struct

	//The event queue that will fill up when we read in from the file
	Node *eventQueue = NULL;

	//Opening the file, if it is not present, the program will know, and end immediately.
	FILE* tasks = fopen("prog3.dat", "r");
	if (tasks == NULL) {
		printf("Cannot find input file, make sure there is a file named prog3.dat in the same directory as the executable.\n");
		return -1;
	}
	//Start to build the event list based on the input file
	while(num > 0) {
		if (fscanf(tasks,"%d %d %d", &pid, &arrival, &time) == 3) { //This will be true if we successfully read in 3 ints, false otherwise
			addInPlace(&eventQueue, makeATask(pid, arrival, time));
		} else {
			if(eventQueue == NULL) {
				printf("Could not read in any tasks from the file.\n");
				printf("Double check formatting and see README.\n");
				return -1;
			} else {
				//If we get here, we have run out of text file, but have not read in SOURCES lines yet.
				// We'll leave it up to the user whether to continue or not
				printf("Ran out of lines to read, %d less than specified.", num);
				char cont;
				do {
					printf("\nDo you want to continue anyway? (y/n) -> ");
					scanf("\n%c", &cont);
					if (cont == 'n') {
						printf("Exiting.\n");
						return -1;
					}
				} while (cont != 'y');
				break;
			}
			return -1;
			}
		num--;
	}
	// End of event list building.
	fclose(tasks);	//Close the input file
	Node *copy = clone(eventQueue); //Clone the event queue, so we can do FCFS with the original and RR with the duplicate
	processFCFS(&eventQueue);
	processRR(&copy, timeSlice);
	printf("Operation successful, logs have been made in RRlog.txt and FCFSlog.txt\n");
	return 0;
}
