#include "../includes/task.h"
#include "../includes/asm_compiler.h"
#include "../includes/helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int PID = 0;

TaskControlBlock* instantiate_tcb(TaskCodeSection* task_code_section, TaskDataSection* task_data_section, int arrival_time, int period) {
    TaskControlBlock* tcb = (TaskControlBlock*) malloc(sizeof(TaskControlBlock));

    if (!tcb) {
        printf("Erro na alocação da memória - Instanciamento do TCB\n");
        return NULL;
    }

    tcb->pid                     = get_next_pid();
    tcb->instructions            = task_code_section->instructions;
    tcb->instruction_count       = task_code_section->instruction_count;
    tcb->labels                  = task_code_section->labels;
    tcb->label_count             = task_code_section->label_count;
    tcb->data                    = task_data_section->variables;
    tcb->data_count              = task_data_section->variable_count;
    tcb->state                   = NEW;

    // EDF config
    tcb->arrival_time            = arrival_time;
    tcb->computation_time        = task_code_section->instruction_count;
    tcb->period                  = period;
    tcb->next_activation_time    = arrival_time;
    tcb->absolute_deadline       = arrival_time + period;
    tcb->remaining_time          = tcb->computation_time;

    // Execução
    tcb->program_counter         = 0;
    tcb->acc                     = 0;
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

void print_tcb(TaskControlBlock* tcb) {
    if (!tcb) {
        printf("TCB nulo, não é possível imprimir.\n");
        return;
    }

    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              TASK CONTROL BLOCK - PID %3zu        ║\n", tcb->pid);
    printf("╠══════════════════════════════════════════════════╣\n");

    const char* states[] = {"NOVO", "EXECUTANDO", "ESPERANDO", "PRONTO", "TERMINADO"};

    printf("║ %-18s: %-28s ║\n", "Estado", states[tcb->state]);
    printf("║ %-18s: %-28zu ║\n", "PC", tcb->program_counter);
    printf("║ %-18s: %-28d ║\n", "Acumulador", tcb->acc);
    printf("║ %-18s: %-28d ║\n", "Chegada", tcb->arrival_time);
    printf("║ %-18s: %-28d ║\n", "Deadline absoluto", tcb->absolute_deadline);
    printf("║ %-19s: %-28d ║\n", "Período", tcb->period);
    printf("║ %-18s: %-28d ║\n", "Tempo bloqueio", tcb->remaining_blocking_time);
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ %-15s: %-33zu ║\n", "# Instruções", tcb->instruction_count);
    printf("║ %-13s: %-33zu ║\n", "# Labels", tcb->label_count);
    printf("║ %-14s: %-33zu ║\n", "# Variáveis", tcb->data_count);
    printf("╚══════════════════════════════════════════════════╝\n");
}

size_t get_next_pid() {
    return PID++;
}

void print_tcb_variables(TaskControlBlock* tcb) {
    if (!tcb) {
        printf("TCB nulo, não é possível imprimir.\n");
        return;
    }

    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║        VARIÁVEIS DO PROCESSO - PID %3zu                 ║\n", tcb->pid);
    printf("╠════════════════════════════════════════════════════════╣\n");
    
    if (tcb->data_count == 0) {
        printf("║             Nenhuma variável definida                 ║\n");
    } else {
        printf("║  %-25s | %-22s    ║\n", "Nome", "Valor");
        printf("╟────────────────────────────┼───────────────────────────╢\n");
        
        for (int i = 0; i < tcb->data_count; i++) {
            printf("║  %-25s | %-22d    ║\n", 
                   tcb->data[i].name, tcb->data[i].value);
        }
    }
    
    printf("╚════════════════════════════════════════════════════════╝\n");
}