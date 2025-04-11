#include "../includes/asm_funcs.h"
#include "../includes/asm_compiler.h"
#include "../includes/task.h"
#include "../includes/helpers.h"
#include <string.h>
#include <stdio.h>

/*
    Arithmetic operations (Operate on the accumulator)
*/
void increment(TaskControlBlock* tcb, int operand) {
    tcb->acc += operand;
}
void decrement(TaskControlBlock* tcb, int operand) {
    tcb->acc -= operand;
}
void multiply(TaskControlBlock* tcb, int operand) {
    tcb->acc *= operand;
}
void divide(TaskControlBlock* tcb, int operand) {
    tcb->acc /= operand;
}

/*
    Memory operations (Operate on the data section)
*/
void load(TaskControlBlock* tcb, char* operand) {
    for (size_t i = 0; i < tcb->data_count; i++) {
        Variable *var = &tcb->data[i];

        if (strcmp(var->name, operand)) {
            tcb->acc = var->value;
            return;
        }
    }
}
void store(TaskControlBlock* tcb, char* operand) {
    for (size_t i = 0; i < tcb->data_count; i++) {
        Variable *var = &tcb->data[i];

        if (strcmp(var->name, operand)) {
            var->value = tcb->acc;
            return;
        }
    }
}

/*
    Jump operations (Jump to labels on instruction set)
*/
void brany(TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, true);
}

void brpos(TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, tcb->acc > 0);
}

void brzero(TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, tcb->acc == 0);
}

void brneg(TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, tcb->acc < 0);
}

/*
    System call
*/
void syscall(TaskControlBlock* tcb, int index) {
    if (index == 0) {
        tcb->state = TERMINATED;
        free_tcb(tcb);
        return;
    }

    if (index == 1) {
        printf("%d", tcb->acc);
    }

    if (index == 2) {
        printf("Syscall 2 -> awaiting for int input\n");
        scanf("%d", &tcb->acc);
    }

    // We set the state to ready because its state is being previously
    // set to waiting before this instruction
    tcb->state = READY;
}

/*
    Branch helper function
*/
void branch_helper(TaskControlBlock *tcb, Label *label, bool condition) {
    if (!condition) {
        return;
    }

    tcb->program_counter = label->mem_pos;
}