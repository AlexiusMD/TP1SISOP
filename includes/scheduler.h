#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct PriorityQueue PriorityQueue;
typedef struct TaskControlBlock TaskControlBlock;

void scheduler_init(PriorityQueue* queue, PriorityQueue* waiting_queue);

void update_blocking_tasks(PriorityQueue* waiting_queue, PriorityQueue* queue);

void run_instruction(TaskControlBlock* tcb);

void check_for_done_tasks(PriorityQueue* queue);

#endif