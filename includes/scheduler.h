#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct PriorityQueue PriorityQueue;
typedef struct TaskControlBlock TaskControlBlock;

void scheduler_init(PriorityQueue* queue, PriorityQueue* waiting_queue);

void update_blocking_tasks(PriorityQueue* waiting_queue, PriorityQueue* queue);

void run_instruction(TaskControlBlock* tcb);

void preempt_priority_handler(TaskControlBlock* current_running_task, PriorityQueue* ready_queue);

void handle_current_task_not_null(TaskControlBlock* current_running_task);

void handle_syscall_instruction(TaskControlBlock* current_running_task, PriorityQueue* waiting_queue);

#endif