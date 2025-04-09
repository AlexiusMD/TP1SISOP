#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE
#define MAX_QUEUE_SIZE 100

#include <task.h>

typedef struct PriorityQueue {
    TaskControlBlock* queue[MAX_QUEUE_SIZE];
    int size;
    int capacity;
} PriorityQueue;

void enqueue(TaskControlBlock* tcb, PriorityQueue* queue);
void dequeue(TaskControlBlock* tcb, PriorityQueue* queue);
void priority_queue_init(int initial_capacity);
void priority_queue_free();

#endif