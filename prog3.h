#include <stdio.h>
#include <stdlib.h>

struct element {
	int pid, arrival,time;
	struct element *next;
};
typedef struct element Node;
