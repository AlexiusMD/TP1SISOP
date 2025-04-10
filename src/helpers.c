#include "../includes/asm_compiler.h"
#include "../includes/task.h"
#include "../includes/helpers.h"
#include "../includes/asm_wrapper.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*
    ASM funcs helper functions
*/

void branch_helper(TaskControlBlock* tcb, Label* label, bool condition) {
    if (!condition) {
        return;
    }

    tcb->program_counter = label->mem_pos;
}
/*
    Compiler helper functions
*/

char** tokenize_sections(char* buffer, char section_name[], int* line_count) {
    char* start_pattern = malloc(strlen(section_name) + 2);
    char* end_pattern = malloc(strlen(section_name) + 5);
    
    if (!start_pattern || !end_pattern) {
        printf("Erro de alocação de memória\n");
        if (start_pattern) free(start_pattern);
        if (end_pattern) free(end_pattern);
        return NULL;
    }

    sprintf(start_pattern, ".%s", section_name);
    sprintf(end_pattern, ".end%s", section_name);

    char* section_start = strstr(buffer, start_pattern);
    char* section_end = strstr(buffer, end_pattern);

    free(start_pattern);
    free(end_pattern);
    
    if (!section_start || !section_end) {
        printf("Missing sections on the file:\n");
        printf("End or start missing - %s\n", section_name);
        BAD_SYNTAX
        return NULL;
    }

    char* section_content_start = strchr(section_start, '\n');
    if (!section_content_start) {
        BAD_SYNTAX
        return NULL;
    }
    section_content_start++;

    size_t section_size = section_end - section_content_start;
    
    char* section_text = (char *) malloc(section_size + 1);
    if (!section_text) {
        printf("Erro de alocação de memória\n");
        return NULL;
    }

    strncpy(section_text, section_content_start, section_size);
    section_text[section_size] = '\0';

    size_t max_possible_lines = section_size / 2 + 1;

    char** tokenized_section = (char**) malloc(max_possible_lines * sizeof(char*));
    if (!tokenized_section) {
        printf("Erro de alocação de memória para tokenized_section\n");
        free(section_text);
        return NULL;
    }

    int count = 0;
    char* linha = strtok(section_text, "\n");
    while (linha && count < max_possible_lines) {
        tokenized_section[count] = strdup(linha);
        if (!tokenized_section[count]) {
            for (int i = 0; i < count; i++) {
                free(tokenized_section[i]);
            }
            free(tokenized_section);
            free(section_text);
            return NULL;
        }
        count++;
        linha = strtok(NULL, "\n");
    }

    if (line_count != NULL) {
        *line_count = count;
    }
    
    free(section_text);

    return tokenized_section;
}

char* strip_whitespace(char* str) {
    if (!str) return NULL;
    
    while (*str && (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\f' || *str == '\v')) {
        str++;
    }
    
    if (!*str) return str;
    
    char* end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\f' || *end == '\v')) {
        *end = '\0';
        end--;
    }
    
    return str;
}

int get_op_code_from_instruction_name(char* name) {
    if(strcmp(name, "add") == 0) {
        return ADD;
    }
    if(strcmp(name, "sub") == 0) {
        return SUB;
    }
    if(strcmp(name, "mult") == 0) {
        return MULT;
    }
    if(strcmp(name, "div") == 0) {
        return DIV;
    }
    if(strcmp(name, "load") == 0) {
        return LOAD;
    }
    if(strcmp(name, "store") == 0) {
        return STORE;
    }
    if(strcmp(name, "brany") == 0) {
        return BRANY;
    }
    if(strcmp(name, "brpos") == 0) {
        return BRPOS;
    }
    if(strcmp(name, "brzero") == 0) {
        return BRZERO;
    }
    if(strcmp(name, "brneg") == 0) {
        return BRNEG;
    }
    if(strcmp(name, "syscall") == 0) {
        return SYSCALL;
    }

    return -1;
}

char* to_lowercase(char* str) {
    if (!str) return NULL;
    
    char* p = str;
    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }
    
    return str;
}

InstructionFn get_instruction_function(OPCode opcode) {
    switch (opcode) {
        case ADD:
            return increment_wrapper;
        case SUB:
            return decrement_wrapper;
        case MULT:
            return multiply_wrapper;
        case DIV:
            return divide_wrapper;
        case STORE:
            return store_wrapper;
        case LOAD:
            return load_wrapper;
        case BRANY:
            return brany_wrapper;
        case BRPOS:
            return brpos_wrapper;
        case BRZERO:
            return brzero_wrapper;
        case BRNEG:
            return brneg_wrapper;
        case SYSCALL:
            return syscall_wrapper;
        default:
            return NULL;
    }
}

int get_opcode_from_function(InstructionFn fn) {
    if (fn == get_instruction_function(ADD)) return ADD;
    if (fn == get_instruction_function(SUB)) return SUB;
    if (fn == get_instruction_function(MULT)) return MULT;
    if (fn == get_instruction_function(DIV)) return DIV;
    if (fn == get_instruction_function(LOAD)) return LOAD;
    if (fn == get_instruction_function(STORE)) return STORE;
    if (fn == get_instruction_function(BRANY)) return BRANY;
    if (fn == get_instruction_function(BRPOS)) return BRPOS;
    if (fn == get_instruction_function(BRZERO)) return BRZERO;
    if (fn == get_instruction_function(BRNEG)) return BRNEG;
    if (fn == get_instruction_function(SYSCALL)) return SYSCALL;
    return -1;
}