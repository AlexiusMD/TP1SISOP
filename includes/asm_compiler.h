#ifndef ASM_COMPILER_H
#define ASM_COMPILER_H

#define BAD_SYNTAX printf("Syntax formatting error\n");

typedef struct TaskControlBlock TaskControlBlock;
typedef unsigned long size_t;

typedef enum OPCode {
    ADD,
    SUB,
    MULT,
    DIV,
    STORE,
    LOAD,
    BRANY,
    BRPOS,
    BRZERO,
    BRNEG,
    SYSCALL
} OPCode;

typedef struct Label {
    char* title;
    size_t mem_pos;
} Label;

typedef struct Variable {
    char* name;
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
    char* operand;
    ParamType type;
} Instruction;

typedef struct TaskCodeSection {
    Instruction* instructions;
    size_t instruction_count;
    Label* labels;
    size_t label_count;
} TaskCodeSection;

typedef struct TaskDataSection {
    Variable* variables;
    size_t variable_count;
} TaskDataSection;

TaskControlBlock* create_task(const char* filename, int arrival_time, int period);
TaskCodeSection* parse_instruction_section(char* instructions_text[], int count);
TaskDataSection* parse_data_section(char* data_text[], int count);

#endif
