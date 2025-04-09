#ifndef ASM_COMPILER_H
#define ASM_COMPILER_H

#include <stdlib.h>
#include <stdbool.h>

#define MAX_LABEL_TITLE_SIZE 255
#define BAD_SYNTAX printf("Syntax formatting error\n");

typedef struct TaskControlBlock TaskControlBlock;

typedef struct Label {
    char title[MAX_LABEL_TITLE_SIZE];
    size_t mem_pos;
} Label;

typedef struct Variable {
    char name[MAX_LABEL_TITLE_SIZE];
    int value;
} Variable;

typedef void (*InstructionFn)(TaskControlBlock*, void*);

typedef enum {
    PARAM_INT,
    PARAM_STRING,
    PARAM_LABEL
} ParamType;

typedef struct Instruction {
    InstructionFn fn;
    void* operand;
    ParamType type;
} Instruction;

TaskControlBlock* parse_program(const char* filename);

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
void syscall    (TaskControlBlock* tcb, int index);

/*
    Helper functions
*/

void branch_helper(TaskControlBlock* tcb, Label* label, bool condition);
char* read_program_file(const char* filename);

#endif
