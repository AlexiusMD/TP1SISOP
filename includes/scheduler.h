#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct PriorityQueue PriorityQueue;
typedef struct TaskControlBlock TaskControlBlock;
typedef unsigned long size_t;

void scheduler_init(PriorityQueue* ready_queue, PriorityQueue* waiting_queue, PriorityQueue* arriving_queue);
void handle_arriving_queue(PriorityQueue* ready_queue, PriorityQueue* arriving_queue, size_t current_time);
void handle_blocked_queue(PriorityQueue* ready_queue, PriorityQueue* waiting_queue);
void handle_ready_queue(PriorityQueue* ready_queue, TaskControlBlock** running_task_ptr);
void run_current_task(TaskControlBlock** current_task_ptr, PriorityQueue* waiting_queue);
void handle_syscalls(TaskControlBlock** current_task_ptr, PriorityQueue* waiting_queue);
void run_instruction(TaskControlBlock* tcb);
void print_queue_states(PriorityQueue* ready_queue, PriorityQueue* waiting_queue, PriorityQueue* arriving_queue);

#endif