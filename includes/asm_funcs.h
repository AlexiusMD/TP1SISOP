#ifndef ASM_FUNCS_H
#define ASM_FUNCS_H

#include <stdbool.h>

typedef struct TaskControlBlock TaskControlBlock;
typedef struct Label Label;

// Instructions included in ASM language

/*
    Arithmetic operations (Operate on the accumulator)
*/
void increment(TaskControlBlock* tcb, int operand);
void decrement(TaskControlBlock* tcb, int operand);
void multiply (TaskControlBlock* tcb, int operand);
void divide   (TaskControlBlock* tcb, int operand);

/*
    Memory operations (Operate on the data section)
*/
void load     (TaskControlBlock* tcb, char* operand);
void store    (TaskControlBlock* tcb, char* operand);

/*
    Jump operations (Jump to labels on instruction set)
*/
void brany     (TaskControlBlock* tcb, Label* label);
void brpos     (TaskControlBlock* tcb, Label* label);
void brzero    (TaskControlBlock* tcb, Label* label);
void brneg     (TaskControlBlock* tcb, Label* label);

/*
    System call
*/
void syscall_    (TaskControlBlock* tcb, int index);

/*
    Helper
*/

void branch_helper(TaskControlBlock* tcb, Label* label, bool condition);
#endif