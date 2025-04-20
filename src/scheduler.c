#include "../includes/scheduler.h"
#include "../includes/task.h"
#include "../includes/priority_queue.h"
#include "../includes/asm_funcs.h"
#include "../includes/asm_compiler.h"
#include "../includes/helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCHEDULER_PERIOD 5 // 5 units of time

void scheduler_init(PriorityQueue *ready_queue, PriorityQueue *waiting_queue)
{
    while (ready_queue->size > 0)
    {
            TaskControlBlock *tcb = ready_queue->heap[0];
            tcb->state = RUNNING;
            run_instruction(tcb);
            tcb->program_counter++;
            tcb->absolute_deadline--;

            printf("Process %zu is executing, pc = %zu; deadline = %d.\n", tcb->pid, tcb->program_counter, tcb->absolute_deadline);
            
            if (tcb->instructions->fn == get_instruction_function(SYSCALL))
            {
                tcb->remaining_blocking_time = rand() % 3 + 1;
                tcb->state = WAITING;
                printf("Process %zu is waiting for %d time units.\n", tcb->pid, tcb->remaining_blocking_time);
                enqueue(tcb, waiting_queue);
                dequeue(ready_queue);
            }

            printf("Priority Queue States:\n");
            printf("====================================\n");
            printf("Ready Queue:\n");
            print_queue(ready_queue);
            printf("=====================================\n");
            printf("Waiting Queue:\n");
            print_queue(waiting_queue);
            printf("=====================================\n");

            //update_blocking_tasks(waiting_queue, queue); // Aconteceu aqui <---- Até aqui rodou

            if (tcb->absolute_deadline == 0){
                tcb->state = TERMINATED;
                dequeue(ready_queue);
            }

        }
        
    }


void update_blocking_tasks(PriorityQueue* waiting_queue, PriorityQueue* queue) {
    if (waiting_queue->size == 0) {
        return;
    }

    for (int i = 0; i < waiting_queue->size; i++) {
        TaskControlBlock* tcb = waiting_queue->heap[i];
        if (tcb->remaining_blocking_time > 0) {
            tcb->remaining_blocking_time--;
        }
        if (tcb->remaining_blocking_time == 0) {
            tcb->state = READY;
            enqueue(tcb, queue);
            dequeue(waiting_queue);
        }
    }
}

void run_instruction(TaskControlBlock* tcb) {
    ParamType instruction_param = tcb->instructions[tcb->program_counter].type;
    int program_counter = tcb->program_counter;

    switch (instruction_param) {
        case PARAM_INT:
            int literal_integer = atoi(tcb->instructions[program_counter].operand);
            tcb->instructions[program_counter].fn(tcb, &literal_integer); 
            break;
        case PARAM_STRING:
            tcb->instructions[program_counter].fn(tcb, tcb->instructions[program_counter].operand);
            break;
        case PARAM_LABEL:
            const char* label = tcb->instructions[program_counter].operand;
            for (int i = 0; i < tcb->label_count; i++) {
                if (strcmp(tcb->labels[i].title, label) == 0) {
                    tcb->instructions[program_counter].fn(tcb, &tcb->labels[i].mem_pos);
                    break;
                }
            }
            break;
        default:
            break;
    }
}