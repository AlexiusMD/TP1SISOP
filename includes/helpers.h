#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>

typedef struct TaskControlBlock TaskControlBlock;
typedef struct Label Label;

/*
    ASM funcs helper functions
*/

void branch_helper(TaskControlBlock* tcb, Label* label, bool condition);

/*
    Compiler helper functions
*/

char** tokenize_sections(char* buffer, char section_name[], int* line_count);
char*  strip_whitespace(char* str);

#endif