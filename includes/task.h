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
    Label* lables;
    Variable* data;
    int8_t state;
    int8_t priority;
    int8_t deadline;
    int8_t remaining_blocking_time;
} TaskControlBlock;

TaskControlBlock* instantiate_tcb(Instruction* instructions, Label* labels, Variable* variables);
void  free_tcb(TaskControlBlock* tcb);
#endif