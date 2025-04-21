#include "../includes/scheduler.h"
#include "../includes/task.h"
#include "../includes/priority_queue.h"
#include "../includes/asm_funcs.h"
#include "../includes/asm_compiler.h"
#include "../includes/helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void scheduler_init(PriorityQueue *ready_queue, PriorityQueue *waiting_queue) {
    size_t current_time = 0;
    TaskControlBlock* running_task = NULL;

    PriorityQueue* arrival_queue = priority_queue_init(10);
    PriorityQueue* aux_queue = priority_queue_init(10);

    while(ready_queue->size > 0) {
        TaskControlBlock* tcb = dequeue(ready_queue);
        if (tcb->arrival_time > current_time) {
            enqueue(tcb, arrival_queue);
        } else {
            enqueue(tcb, aux_queue);
        }
    }

    ready_queue = aux_queue;

    while (ready_queue->size > 0 || waiting_queue->size > 0) {
        printf("\n===== Tempo atual: %zu =====\n", current_time);

        if (arrival_queue->size == 0) {
            priority_queue_free(arrival_queue);
        }

        if (arrival_queue->size > 0) {
            if (peek(arrival_queue)->arrival_time == current_time) { 
                TaskControlBlock* arrived_task = dequeue(arrival_queue);
                arrived_task->state = READY;
                printf("Tarefa %zu chegou no sistema no tempo %zu\n", arrived_task->pid, current_time);
                enqueue(arrived_task, ready_queue);
            }
        }
        // ----------------------------------- ACIMA DAQUI TÁ CERTO // olha a funcao q eu to fazendo aq em baixo
        
        if (running_task != NULL && running_task->absolute_deadline < current_time) {
            printf("Tarefa %zu perdeu o seu deadline! Deadline = %d, Tempo atual = %zu\n", running_task->pid, running_task->absolute_deadline, current_time);
            running_task->state = READY;
            running_task->absolute_deadline += running_task->period;
            running_task = NULL;
        }

        if(waiting_queue->size > 0) {
            update_blocking_tasks(waiting_queue, ready_queue);
        }

        if(ready_queue->size > 0 ) {
            if(running_task != NULL) {
                preempt_priority_handler(running_task, ready_queue);
            }
    
            running_task = dequeue(ready_queue);
            running_task->state = RUNNING;
            printf("Nova tarefa executando: tarefa %zu rodando. Deadline = %d.\n", running_task->pid, running_task->absolute_deadline);
        }

        if (running_task != NULL) {
            // run_instruction(running_task);
            // running_task->program_counter++;
            // running_task->remaining_time--;

            // printf("Tarefa %zu executou instrução %zu. Restantes: %d\n", running_task->pid, running_task->program_counter-1, running_task->remaining_time);

            if (running_task->program_counter < running_task->instruction_count && running_task->instructions[running_task->program_counter].fn == get_instruction_function(SYSCALL)) {
                if (0 == atoi(running_task->instructions[running_task->program_counter].operand)) {
                    printf("Tarefa %zu terminou execução\n", running_task->pid);        
                    running_task->state = TERMINATED;            
                    running_task = NULL;
                } else {
                    running_task->remaining_blocking_time = rand() % 3 + 1;
                    running_task->state = WAITING;
                    enqueue(running_task, waiting_queue);
                    printf("Tarefa %zu bloqueada por %d unidades de tempo\n", running_task->pid, running_task->remaining_blocking_time);
                    running_task = NULL;
                }
            }

            if (running_task != NULL && running_task->remaining_time <= 0) {
                printf("Tarefa %zu completou sua execução\n", running_task->pid);
                running_task->state = TERMINATED;
                running_task = NULL;
            }
        }

        current_time++;
    }

    priority_queue_free(aux_queue);
    printf("\nTodas as tarefas foram concluídas no tempo %zu\n", current_time);
}

void handle_current_task_not_null(TaskControlBlock* current_running_task, size_t* current_time, PriorityQueue* waiting_queue) {
    if (current_running_task->absolute_deadline < current_time) {
        printf("Tarefa %zu perdeu o seu deadline! Deadline = %d\n", current_running_task->pid, current_running_task->absolute_deadline);
        current_running_task->state = READY;
        current_running_task->absolute_deadline += current_running_task->period;
        current_running_task = NULL;
        return;
    }
    
    run_instruction(current_running_task);
    current_running_task->program_counter++;
    current_running_task->remaining_time--;
    printf("Tarefa %zu executou instrução %zu. Restantes: %d\n", current_running_task->pid, current_running_task->program_counter-1, current_running_task->remaining_time);

    if (current_running_task->program_counter == current_running_task->instruction_count && current_running_task->remaining_time <= 0) {
        printf("Tarefa %zu completou sua execução\n", current_running_task->pid);
        current_running_task->state = TERMINATED;
        return;
    }

    if (0 == atoi(current_running_task->instructions[current_running_task->program_counter].operand)) {
        printf("Tarefa %zu completou sua execução\n", current_running_task->pid);     
        current_running_task->state = TERMINATED;            
        current_running_task = NULL;
        return;
    }
    
} // Se cair ou algo assim me avisa que eu dou push pra tu fazer no teu, vou comer, brb..., ta

void handle_syscall_instruction(TaskControlBlock* current_running_task) {
    if (current_running_task->instructions[current_running_task->program_counter].fn == get_instruction_function(SYSCALL)) {

        if (0 == atoi(current_running_task->instructions[current_running_task->program_counter].operand)) {
            printf("Tarefa %zu completou sua execução\n", current_running_task->pid);     
            current_running_task->state = TERMINATED;
            current_running_task = NULL;
            return;
        }

        current_running_task->remaining_blocking_time = rand() % 3 + 1;
        current_running_task->state = WAITING;
        enqueue(current_running_task, waiting_queue);
        printf("Tarefa %zu bloqueada por %d unidades de tempo\n", current_running_task->pid, current_running_task->remaining_blocking_time);
        current_running_task = NULL;
        return;
    }
}


void preempt_priority_handler(TaskControlBlock* current_running_task, PriorityQueue* ready_queue) {
    current_running_task->state = READY; 
    enqueue(current_running_task, ready_queue);
    printf("Tarefa preemptada: %zu interrompida\n", current_running_task->pid);
    current_running_task = NULL;
}

void update_blocking_tasks(PriorityQueue* waiting_queue, PriorityQueue* ready_queue) {
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
            enqueue(tcb, ready_queue);
            dequeue(waiting_queue);
        }
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