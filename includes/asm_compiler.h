#ifndef ASM_COMPILER_H
#define ASM_COMPILER_H

#include <stdlib.h>

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

#endif
