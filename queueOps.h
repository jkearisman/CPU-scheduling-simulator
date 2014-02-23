Node *clone(Node *list);
void addInPlace(Node **list, Node *newProc);
void wipeFrontProc(Node **list);
void printQueue(Node *list);
void addToBackOfQueue(Node **list, Node *newProc);
void pushFrontToBack(Node **list);
void addToFrontOfQueue(Node **list, Node *newProc); 
Node *makeATask(int pid, int arrival, int time);
