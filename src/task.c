#include "../includes/task.h"
#include "../includes/asm_compiler.h"
#include <stdio.h>

TaskControlBlock* instantiate_tcb(Instruction* instructions, Label* labels, Variable* variables, size_t pid) {
    TaskControlBlock* tcb = (TaskControlBlock*) malloc(sizeof(TaskControlBlock));
    if (!tcb) {
        printf("Erro na alocação da memória - Instanciamento do TCB\n");
        return NULL;
    }

    tcb->pid                     = pid;
    tcb->instructions            = instructions;
    tcb->labels                  = labels;
    tcb->data                    = variables;
    tcb->state                   = NEW;
    tcb->deadline                = 0;
    tcb->acc                     = 0;
    tcb->priority                = 0;
    tcb->remaining_blocking_time = 0;

    return tcb;
}

void free_tcb(TaskControlBlock* tcb) {
    if (tcb->instructions) {
        for (int i = 0; i < tcb->instruction_count; i++) {
            if (tcb->instructions[i].operand) {
                free(tcb->instructions[i].operand);
            }
        }
        free(tcb->instructions);
    }
    
    if (tcb->labels) {
        for (int i = 0; i < tcb->label_count; i++) {
            if (tcb->labels[i].title) {
                free(tcb->labels[i].title);
            }
        }
        free(tcb->labels);
    }
    
    if (tcb->data) {
        for (int i = 0; i < tcb->data_count; i++) {
            if (tcb->data[i].name) {
                free(tcb->data[i].name);
            }
        }
        free(tcb->data);
    }
    
    free(tcb);
}