#ifndef ASM_COMPILER_H
#define ASM_COMPILER_H

#include <stdlib.h>

#define MAX_TITLE_SIZE 256 // (2^8) chars + 1 null byte
#define BAD_SYNTAX printf("Syntax formatting error\n");

typedef struct TaskControlBlock TaskControlBlock;

typedef struct Label {
    char title[MAX_TITLE_SIZE];
    size_t mem_pos;
} Label;

typedef struct Variable {
    char name[MAX_TITLE_SIZE];
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
Instruction* parse_instructions(char* instructions_text[], int count);
char** tokenize_sections(char* buffer, char section_name[], int* line_count);

#endif
