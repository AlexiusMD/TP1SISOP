#ifndef ASM_WRAPPER_H
#define ASM_WRAPPER_H

typedef struct TaskControlBlock TaskControlBlock;

void increment_wrapper(TaskControlBlock* tcb, void* operand);
void decrement_wrapper(TaskControlBlock* tcb, void* operand);
void multiply_wrapper (TaskControlBlock* tcb, void* operand);
void divide_wrapper   (TaskControlBlock* tcb, void* operand);

/*
    Memory operations (Operate on the data section)
*/
void load_wrapper     (TaskControlBlock* tcb, void* operand);
void store_wrapper    (TaskControlBlock* tcb, void* operand);

/*
    Jump operations (Jump to labels on instruction set)
*/
void brany_wrapper     (TaskControlBlock* tcb, void* operand);
void brpos_wrapper     (TaskControlBlock* tcb, void* operand);
void brzero_wrapper    (TaskControlBlock* tcb, void* operand);
void brneg_wrapper     (TaskControlBlock* tcb, void* operand);

/*
    System call
*/
void syscall_wrapper    (TaskControlBlock* tcb, void* index);

#endif