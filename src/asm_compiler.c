#include "../includes/asm_compiler.h"
#include "../includes/asm_funcs.h"
#include "../includes/file_reader.h"
#include "../includes/task.h"
#include <stdio.h>
#include <string.h>

Instruction* parse_instructions(char instructions_text[], int count) {
    Instruction* result = (Instruction*)malloc(count * sizeof(Instruction));
    if (!result) {
        printf("Memory allocation error - Instructions array\n");
        return NULL;
    }

    return result;
}

TaskControlBlock* parse_program(const char* filename) {
    char* buffer = read_program_file(filename);

    char* instructions_start = strstr(buffer, ".code");
    char* instructions_end = strstr(buffer, ".endcode");
    char* data_start = strstr(buffer, ".data");
    char* data_end = strstr(buffer, ".enddata");

    if (!instructions_start || !instructions_end || !data_start || !data_end) {
        printf("Missing sections on the file:\n");
        if (!instructions_start) printf("- .code\n");
        if (!instructions_end) printf("- .endcode\n");
        if (!data_start) printf("- .data\n");
        if (!data_end) printf("- .enddata\n");
        
        BAD_SYNTAX
        free(buffer);
        return NULL;
    }

    size_t code_size = instructions_end - instructions_start;
    char* code_section = (char*)malloc(code_size + 1);
    if (!code_section) {
        printf("Memory allocation error - Code section\n");
        free(buffer);
        return NULL;
    }

    char* code_content_start = strchr(instructions_start, '\n');
    if (!code_content_start) {
        BAD_SYNTAX
        free(buffer);
        free(code_section);
        return NULL;
    }
    code_content_start++;

    code_size = instructions_end - code_content_start;
    strncpy(code_section, code_content_start, code_size);
    code_section[code_size] = '\0';

    size_t data_size = data_end - data_start;
    char* data_section = (char*)malloc(data_size + 1);
    if (!data_section) {
        printf("Memory allocation error - Data section\n");
        free(buffer);
        free(code_section);
        return NULL;
    }

    char* data_content_start = strchr(data_start, '\n');
    if (!data_content_start) {
        BAD_SYNTAX
        free(buffer);
        free(code_section);
        free(data_section);
        return NULL;
    }
    data_content_start++;

    data_size = data_end - data_content_start;
    strncpy(data_section, data_content_start, data_size);
    data_section[data_size] = '\0';

    char* instructions[MAX_BUFFER_SIZE];
    int count = 0;

    char* linha = strtok(code_section, "\n");
    while (linha && count < MAX_BUFFER_SIZE) {
        instructions[count++] = linha;
        linha = strtok(NULL, "\n");
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", instructions[i]);
    }

    Instruction* task_instructions = parse_instructions(code_section, count);

    free(task_instructions);
    
    char* data[MAX_BUFFER_SIZE];
    count = 0;
    
    linha = strtok(data_section, "\n");
    while (linha && count < MAX_BUFFER_SIZE) {
        data[count++] = linha;
        linha = strtok(NULL, "\n");
    }
    
    for (int i = 0; i < count; i++) {
        printf("%s\n", data[i]);
    }

    TaskControlBlock* tcb = (TaskControlBlock*) malloc(sizeof(TaskControlBlock));

    if (!tcb) {
        printf("Memory allocation error - TaskControlBlock\n");
        free(buffer);
        free(code_section);
        free(data_section);
        return NULL;
    }

    free(buffer);
    free(code_section);
    free(data_section);

    return tcb;
}

/*
    Arithmetic operations (Operate on the accumulator)
*/
void increment(TaskControlBlock* tcb, int operand) {
    tcb->acc += operand;
}
void decrement(TaskControlBlock* tcb, int operand) {
    tcb->acc -= operand;
}
void multiply (TaskControlBlock* tcb, int operand) {
    tcb->acc *= operand;
}
void divide   (TaskControlBlock* tcb, int operand) {
    tcb->acc /= operand;
}

/*
    Memory operations (Operate on the data section)
*/
void load     (TaskControlBlock* tcb, char* operand) {
    for(size_t i = 0; i < MAX_BUFFER_SIZE; i++) {
        Variable *var = &tcb->data[i];

        if(strcmp(var->name, operand)) {
            tcb->acc = var->value;
            return;
        }
    }
}
void store    (TaskControlBlock* tcb, char* operand) {
    for(size_t i = 0; i < MAX_BUFFER_SIZE; i++) {
        Variable *var = &tcb->data[i];

        if(strcmp(var->name, operand)) {
            var->value = tcb->acc;
            return;
        }
    }
}

/*
    Jump operations (Jump to labels on instruction set)
*/
void brany     (TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, true);
}

void brpos     (TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, tcb->acc > 0);
}

void brzero    (TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, tcb->acc == 0);
}

void brneg     (TaskControlBlock* tcb, Label* label) {
    branch_helper(tcb, label, tcb->acc < 0);
}

void branch_helper(TaskControlBlock* tcb, Label* label, bool condition) {
    if (!condition) {
        return;
    }

    tcb->program_counter = label->mem_pos;
}

/*
    System call
*/
void syscall    (TaskControlBlock* tcb, int index);