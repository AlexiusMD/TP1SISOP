#ifndef TASK_H
#define TASK_H

#include <stdlib.h>
#include <stdint.h>

typedef enum TaskStates {
    RUNNING,
    WAITING,
    READY,
    TERMINATED
} TaskStates;

typedef struct Label Label;
typedef struct Variable Variable;
typedef struct Instruction Instruction;
typedef struct TaskCodeSection TaskCodeSection;
typedef struct TaskDataSection TaskDataSection;

typedef struct TaskControlBlock {
    size_t pid; // Process ID
    TaskStates state; // Estado atual do processo

    // ----------------------------
    // Registradores
    // ----------------------------
    int32_t acc;
    size_t program_counter;

    // ----------------------------
    //  Instruções e dados
    // ----------------------------
    Instruction* instructions;
    size_t instruction_count;
    Label* labels;
    size_t label_count;
    Variable* data;
    size_t data_count;


    // ----------------------------
    // Campos para escalonador
    // ----------------------------
    int arrival_time;
    int computation_time;
    int period;
    int next_activation_time;
    int absolute_deadline;
    int remaining_time;
    int remaining_blocking_time;
} TaskControlBlock;

int calculate_deadline(Instruction* instructions, size_t instruction_count);

TaskControlBlock* instantiate_tcb(TaskCodeSection* task_code_section, TaskDataSection* task_data_section, int arrival_time, int period);
void free_tcb(TaskControlBlock* tcb);
size_t get_next_pid();
void print_tcb(TaskControlBlock* tcb);
#endif