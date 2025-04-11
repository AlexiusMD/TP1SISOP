#ifndef TASK_H
#define TASK_H

#include <stdlib.h>
#include <stdint.h>

typedef enum TaskStates {
    NEW,
    RUNNING,
    WAITING,
    READY,
    TERMINATED
} TaskStates;

typedef struct Label Label;
typedef struct Variable Variable;
typedef struct Instruction Instruction;

typedef struct TaskControlBlock {
    size_t pid;
    int32_t acc;
    size_t program_counter;
    Instruction* instructions;
    size_t instruction_count;
    Label* labels;
    size_t label_count;
    Variable* data;
    size_t data_count;
    TaskStates state;
    int8_t priority;
    int8_t deadline;
    int8_t remaining_blocking_time;
} TaskControlBlock;

int calculate_deadline(Instruction* instructions, size_t instruction_count);

TaskControlBlock* instantiate_tcb(Instruction* instructions, Label* labels, Variable* variables, size_t pid);
void free_tcb(TaskControlBlock* tcb);
#endif