Jesse Earisman
cs2303-c03

This is a program to simulate two CPU scheduling models, first come first served (FCFS) and round robin (RR).

compile with: make
run with: ./prog3 $LINES $TIMESLICE
	where:
	$LINES is the number of lines to be read in from the input file (must be at least 1)
	$TIMESLICE is the timeslice to use for the RR processing (must be at least 1)

The program reads in from a file with the name "prog3.dat." Every line of this file should by formatted in
	pid arrival time
where pid is the process id, arrival is the time that this process arrives, and time is the time it takes the CPU to process this task.
All times should be in units of 100ms (sims). And pid, arrival, and time should all be integers (no decimals)
Example:
A process with pid 1, that arrives 400ms after the program starts, and takes 1000ms(1sec) to process fully would look like:
1 4 10 

This program reads in the file and turns each set of 3 ints into a process struct, and adds it to an event queue.
This queue will be processed with FCFS and RR, to see the differences

Each new element of the event list is add in order, according to these simple rules:
1) Earliest arriving tasks always precede tasks that arrive later.
2) If two tasks arrive at the same time, the task that will process faster goes first
3) If two tasks arrive at the same time and take the same amount of time to process, whichever task is entered later in the input file will go first

The final queue is already sorted and ready to be processed.

The program clones the queue, so we can use it multiple times, then start processing.
Both methods continue processing until there are no more events, sometimes this means that they sit idle for a little while.

First, it uses FCFS.
In FCFS, the first process entered is processed to completion, then the CPU moves on to the next process.
The log of the FCFS processing is stored in the text file FCFSlog.txt

Next, it uses RR.
In RR, each process recieves a certian amount of time (specifically the $TIMESLICE argument given when the program is called)
After this time, if the process is not completed, it is moved to the back of the queue, to await more time.
This continues until there are no more processes.
The log of the RR processing is stored in RRlog.txt

Known bugs:
