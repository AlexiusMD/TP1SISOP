#include "../includes/task.h"
#include "../includes/asm_compiler.h"
#include "../includes/helpers.h"
#include <stdio.h>

static int PID = 0;

TaskControlBlock* instantiate_tcb(TaskCodeSection* task_code_section, TaskDataSection* task_data_section) {
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
    tcb->deadline                = calculate_deadline(task_code_section->instructions, task_code_section->instruction_count);
    tcb->program_counter         = 0;
    tcb->acc                     = 0;
    tcb->priority                = 0;
    tcb->remaining_blocking_time = 0;

    return tcb;
}

int calculate_deadline(Instruction* instructions, size_t instruction_count) {
    int deadline = 0;

    for (size_t i = 0; i < instruction_count; i++) {
        if (get_opcode_from_function(instructions[i].fn) == SYSCALL) {
            // We assume the worst computational time for a syscall, which is 3 cycles
            deadline += 3;
        }

        deadline++;
    }
    
    return deadline;
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

    printf("║ %-15s: %-31s ║\n", "Estado", states[tcb->state]);
    printf("║ %-15s: %-31zu ║\n", "PC", tcb->program_counter);
    printf("║ %-15s: %-31d ║\n", "Acumulador", tcb->acc);
    printf("║ %-15s: %-31d ║\n", "Prioridade", tcb->priority);
    printf("║ %-15s: %-31d ║\n", "Deadline", tcb->deadline);
    printf("║ %-15s: %-31d ║\n", "Tempo bloqueio", tcb->remaining_blocking_time);
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║ %-15s: %-33zu ║\n", "# Instruções", tcb->instruction_count);
    printf("║ %-13s: %-33zu ║\n", "# Labels", tcb->label_count);
    printf("║ %-14s: %-33zu ║\n", "# Variáveis", tcb->data_count);
    printf("╚══════════════════════════════════════════════════╝\n");
}

size_t get_next_pid() {
    return PID++;
}