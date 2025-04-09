#include "../includes/asm_compiler.h"
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
        printf("%s\n", instructions_text[i]);
    }

    return result;
}

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