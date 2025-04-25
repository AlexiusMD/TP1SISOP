#ifndef HELPERS_H
#define HELPERS_H

typedef struct TaskControlBlock TaskControlBlock;
typedef struct Label Label;

/*
    Compiler helper functions
*/

char** tokenize_sections(char* buffer, char section_name[], int* line_count);
char*  strip_whitespace(char* str);
int    get_op_code_from_instruction_name(char* name);
char*   to_lowercase(char* str);
InstructionFn get_instruction_function(OPCode opcode);
int get_opcode_from_function(InstructionFn fn);
const char* get_instruction_name_from_opcode(OPCode opcode);

#endif