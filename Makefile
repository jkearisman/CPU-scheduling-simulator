make: prog3 clean

prog3: prog3.o queueOps.o process.o
	gcc -lm -Wall prog3.o queueOps.o process.o -o prog3

lab2.o:
	gcc -c -Wall -o prog3.o prog3.c

queueOps.o:
	gcc -c -Wall -o queueOps.o queueOps.c
process.o:
	gcc -lm -c -Wall -o process.o process.c

clean:
	rm *.o
