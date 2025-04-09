#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct PriorityQueue PriorityQueue;

void scheduler_init(PriorityQueue* queue, PriorityQueue* waiting_queue);

#endif