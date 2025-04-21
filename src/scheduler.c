#include "../includes/scheduler.h"
#include "../includes/task.h"
#include "../includes/priority_queue.h"
#include "../includes/asm_funcs.h"
#include "../includes/asm_compiler.h"
#include "../includes/helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void scheduler_init(PriorityQueue* ready_queue, PriorityQueue* waiting_queue, PriorityQueue* arriving_queue) {
    size_t current_time = 0;
    TaskControlBlock* running_task = NULL;

    while (ready_queue->size > 0 || waiting_queue->size > 0 || arriving_queue->size > 0 || running_task != NULL) {
        print_queue_states(ready_queue, waiting_queue, arriving_queue);
        handle_arriving_queue(ready_queue, arriving_queue, current_time);
        handle_blocked_queue(ready_queue, waiting_queue);
        handle_ready_queue(ready_queue, &running_task);
        run_current_task(&running_task, waiting_queue);
        current_time++;
        if (arriving_queue->size == 0 && ready_queue->size == 0 && waiting_queue->size == 0 && running_task == NULL) {
            break;
        }
    }

    printf("\nTodas as tarefas foram concluídas no tempo %zu\n", current_time);
}

void print_queue_states(PriorityQueue* ready_queue, PriorityQueue* waiting_queue, PriorityQueue* arriving_queue) {
    printf("\n══════════════════════════════════════════════════\n");
    printf("Estado das filas:");
    printf("\n══════════════════════════════════════════════════\n");
    printf("Fila de Prontos: ");
    print_queue(ready_queue);
    printf("\n══════════════════════════════════════════════════\n");
    printf("Fila de Espera: ");
    print_queue(waiting_queue);
    printf("\n══════════════════════════════════════════════════\n");
    printf("Fila de Chegada: ");
    print_queue(arriving_queue);
    printf("\n══════════════════════════════════════════════════\n");
}

void handle_arriving_queue(PriorityQueue* ready_queue, PriorityQueue* arriving_queue, size_t current_time) {
    if(arriving_queue->size == 0) {
        return;
    }

    while (arriving_queue->size > 0 && peek(arriving_queue)->arrival_time <= current_time) {
        TaskControlBlock* arrived_task = dequeue(arriving_queue);
        arrived_task->state = READY;
        printf("Tarefa %zu chegou no sistema no tempo %zu\n", arrived_task->pid, current_time);
        enqueue(arrived_task, ready_queue);
    }
}

void handle_blocked_queue(PriorityQueue* ready_queue, PriorityQueue* waiting_queue) {
    if (waiting_queue->size == 0) {
        return;
    }

    while (waiting_queue->size > 0) {
        TaskControlBlock* blocked_task = peek(waiting_queue);

        blocked_task->remaining_blocking_time--;
        
        if (blocked_task->remaining_blocking_time <= 0) {
            dequeue(waiting_queue);
            blocked_task->state = READY;
            printf("Tarefa %zu desbloqueada\n", blocked_task->pid);
            enqueue(blocked_task, ready_queue);
        } else {
            break;
        }
    }
}

void handle_ready_queue(PriorityQueue* ready_queue, TaskControlBlock** running_task_ptr) {
    TaskControlBlock* running_task = *running_task_ptr;

    if(ready_queue->size == 0) {
        return;
    }

    if (running_task == NULL || peek(ready_queue)->absolute_deadline < running_task->absolute_deadline) {
        if (running_task != NULL) {
            running_task->state = READY;
            enqueue(running_task, ready_queue);
            printf("Tarefa preemptada: %zu interrompida\n", running_task->pid);
        }
    
        running_task = dequeue(ready_queue);
        running_task->state = RUNNING;
        printf("Tarefa %zu selecionada (deadline: %d)\n", running_task->pid, running_task->absolute_deadline);
        *running_task_ptr = running_task;
    }
}

void run_current_task(TaskControlBlock** current_task_ptr, PriorityQueue* waiting_queue) {
    TaskControlBlock* current_task = *current_task_ptr;

    if (current_task == NULL) {
        return;
    }

    run_instruction(current_task);
    current_task->program_counter++;
    printf("Tarefa %zu executou instrução %zu\n", current_task->pid, current_task->program_counter - 1);

    if (current_task->program_counter >= current_task->instruction_count) {
        printf("Tarefa %zu completou todas as instruções\n", current_task->pid);
        current_task->state = TERMINATED;
        *current_task_ptr = NULL;
        return;
    }

    if (current_task->instructions[current_task->program_counter].fn != get_instruction_function(SYSCALL)) {
        return;
    }
    
    handle_syscalls(current_task_ptr, waiting_queue);
}

void handle_syscalls(TaskControlBlock** current_task_ptr, PriorityQueue* waiting_queue) {
    TaskControlBlock* current_task = *current_task_ptr;

    int syscall_value = atoi(current_task->instructions[current_task->program_counter].operand);
    
    if (syscall_value == 0) {
        printf("Tarefa %zu finalizou com syscall(0)\n", current_task->pid);
        current_task->state = TERMINATED;
        *current_task_ptr = NULL;
    } else {
        current_task->remaining_blocking_time = rand() % 3 + 1;
        current_task->state = WAITING;
        printf("Tarefa %zu bloqueada por %d unidades de tempo\n", current_task->pid, current_task->remaining_blocking_time);
        enqueue(current_task, waiting_queue);
        *current_task_ptr = NULL;
    }
}

void run_instruction(TaskControlBlock* tcb) {
    ParamType instruction_param = tcb->instructions[tcb->program_counter].type;
    int program_counter = tcb->program_counter;
    int literal_integer;
    const char* label;

    switch (instruction_param) {
        case PARAM_INT:
            literal_integer = atoi(tcb->instructions[program_counter].operand);
            tcb->instructions[program_counter].fn(tcb, &literal_integer); 
            break;
        case PARAM_STRING:
            tcb->instructions[program_counter].fn(tcb, tcb->instructions[program_counter].operand);
            break;
        case PARAM_LABEL:
            label = tcb->instructions[program_counter].operand;
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