#include "../includes/asm_compiler.h"
#include "../includes/helpers.h"
#include "../includes/asm_funcs.h"
#include "../includes/file_reader.h"
#include "../includes/task.h"
#include <stdio.h>
#include <string.h>

TaskControlBlock* parse_program(const char* filename) {
    char* buffer = read_program_file(filename);

    int count = 0;
    char** code_section = tokenize_sections(buffer, "code", &count);

    Instruction* task_instructions = parse_instructions(code_section,  count);

    free(task_instructions);

    count = 0;
    char** data_section = tokenize_sections(buffer, "data", &count);

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

Instruction* parse_instructions(char* instructions_text[], int count) {
    Instruction* result = (Instruction*)malloc(count * sizeof(Instruction));
    if (!result) {
        printf("Memory allocation error - Instructions array\n");
        return NULL;
    }

    for(int i = 0; i < count; i++) {
        instructions_text[i] = strip_whitespace(instructions_text[i]);
        printf("%s;", instructions_text[i]);
    }

    printf("\n");

    return result;
}