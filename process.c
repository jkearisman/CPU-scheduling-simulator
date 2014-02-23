#include <math.h>
#include "prog3.h"
#include "process.h"
#include "queueOps.h"

void printLog(FILE *file, Node *logQueue, int totalTime) {
	Node* focus = logQueue;
	int numEvents = 0;
	int responseTime;
	int cumResponseTime = 0;
	int quickResponse = 9999;
	int slowResponse = 0;
	int mean;
	//float stdDev;
	int errSum = 0;
	int temp;
	fprintf(file, "\nTable of processes, in order of completion:\n");	
	fprintf(file, "PID\tArrival\tFinish\tResponse Time\n");
	while(focus != NULL) {
		responseTime = focus->time - focus -> arrival;
		if (responseTime > slowResponse)
			slowResponse = responseTime;
		if(responseTime < quickResponse)
			quickResponse = responseTime;
		cumResponseTime += responseTime;
		fprintf(file, "%d\t%.1fs\t%.1fs\t%.1fs\n", focus -> pid, .1*focus -> arrival, .1*focus -> time, .1*responseTime);
		focus = focus -> next;
		numEvents++;
	}
	mean = 1.0*cumResponseTime/numEvents;
	fprintf(file, "\nStats:\n");
	fprintf(file, "%d processes completed in %.1fs\n", numEvents, .1*totalTime);
	fprintf(file, "Fastest response time was %.1fs\n", .1*quickResponse);
	fprintf(file, "Slowest response time was %.1fs\n", .1*slowResponse);
	fprintf(file, "Average response time was %.1fs\n", .1*mean);

	//Caluclating standard deviation
	focus = logQueue;
	while(focus != NULL) {
		temp = mean - (focus -> time - focus -> arrival);
		errSum += temp * temp;
		focus = focus -> next;
	}
	fprintf(file, "Standard deviation (in seconds) is %.2f\n", sqrt(.01*errSum/--numEvents));
	

}

/*
Loads any processes that arrive at the given time
Pops them out of the event queue and puts them at the back of the process queue
This is capable of adding multiple events if they all arrive at the same time
*/
void loadProcs(Node **eventQ, Node **procQ, int totalTime, FILE *file) {
	Node *nextEvent = *eventQ;
	while(nextEvent != NULL && nextEvent -> arrival <= totalTime) {
		*eventQ = nextEvent -> next;
		addToBackOfQueue(procQ, nextEvent);
                fprintf(file,"Added process %d to process queue at T=%d\n", nextEvent -> pid, totalTime);
                nextEvent = *eventQ; //Update next event, check again
        }

}
/*
Process the given event queue
The main process function, is capable of either FCFS or RR
if the given timeSlice is zero, it uses FCFS, otherwise, it uses RR with a timeSlice
        equal to the given
Every run through of the program, it gives a printout with the simulated time, (in 100ms) and current status
	this printout is directed into whatever file is passed to the program
*/
void process(Node **eventQ, int timeSlice, FILE *file) {
	Node *procQ = NULL; 	//The queue where we will store arrived tasks until they are processed
        int totalTime = 0;	//Variable to keep total time
        int time = timeSlice;	//Variable used in RR, to keep track of when a process has used up its timeSlice
	
	//Log queue. Events are put in this when they are completed
	// Only difference is that the time variable is used to keep a record of when the process finishes		
	Node *logQueue = NULL; 

	//Keep going until we've process every task        
	while(*eventQ != NULL || procQ != NULL) {

		//If anything arrives during this sim, we add it to the back of the process queue and update the event queue
		loadProcs(eventQ, &procQ, totalTime, file);

		//Start of processing section
                Node *currProc = procQ;
		//If we have something to process
                if (currProc != NULL) {
                        if(timeSlice != 0) //If we aren't in FCFS mode
                                time--;

                        currProc-> time--; //this tasks remaining time is reduced by one as the CPU processes one sim
                        
			if(currProc -> time <= 0) { 					//If this task is done
                                fprintf(file, "Finished process %d at T=%d\n",currProc->pid, totalTime);
				currProc -> time = totalTime;
				procQ = currProc -> next;
                                addToBackOfQueue(&logQueue, currProc);					//Clear the process, deallocate memory, shorten process queue
                                time = timeSlice;					//Reset time slice
                        } 

			else if (timeSlice != 0 && time <= 0) { //If we are in RR mode and the timeSlice is done
                                pushFrontToBack(&procQ);
                                time = timeSlice; //Reset timeslice
                        }
                }
		//End of processing section
	
                totalTime++;
        }
	printLog(file, logQueue, totalTime);

	//Cleaning up, all memory should be freed now
	while(logQueue != NULL)
		wipeFrontProc(&logQueue);
	return;

}


/*
Process the given event queue in FCFS mode
opens the text file we want to log to, and calls the master process function with a timeSlice of 0, the sentinel for FCFS
*/
void processFCFS(Node **eventQ) {
        FILE *logFile = fopen("FCFSlog.txt", "w");
	process(eventQ, 0, logFile);
	fclose(logFile);
}


/*
Processes the given event list in Round robin mode
opens the RR text file, and calls the process function
*/
void processRR(Node **eventQ, int timeSlice) {
	FILE *logFile = fopen("RRlog.txt", "w");
	process(eventQ, timeSlice, logFile);
	fclose(logFile);
}
