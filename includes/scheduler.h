#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct PriorityQueue PriorityQueue;
typedef struct TaskControlBlock TaskControlBlock;

void scheduler_init(PriorityQueue* queue, PriorityQueue* waiting_queue);

#endif