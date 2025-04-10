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

    TaskCodeSection* task_instruction_section = parse_instruction_section(code_section,  count);

    free(task_instruction_section);

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

TaskCodeSection* parse_instruction_section(char* instructions_text[], int count) {
    int instruction_count = 0;
    int label_count = 0;

    for(int i = 0; i < count; i++) {
        char* line = strip_whitespace(instructions_text[i]);
        if (!line || strlen(line) == 0) continue;
        
        if (line[strlen(line) - 1] == ':') {
            label_count++;
            continue;
        }

        instruction_count++;
    }

    TaskCodeSection* task_code_section = (TaskCodeSection*) malloc(sizeof(TaskCodeSection));
    if(!task_code_section) {
        printf("Erro na alocação de memória - Task Code Section struct\n");
        return NULL;
    }

    Instruction* instructions = (Instruction*) malloc(instruction_count * sizeof(Instruction));
    if (!instructions) {
        printf("Erro na alocação de memória - Array de Instruções\n");
        free(task_code_section);
        return NULL;
    }

    Label* labels = (Label*) malloc(label_count * sizeof(Label));
    if (!labels) {
        printf("Erro na alocação de memória - Array de Labels\n");
        free(task_code_section);
        free(instructions);
        return NULL;
    }

    int current_instruction_index = 0;
    int current_label_index = 0;
    
    for(int i = 0; i < count; i++) {
        char* line = strip_whitespace(instructions_text[i]);

        size_t len = strlen(line);
        if (!line || len == 0) continue;
        
        if (len > 0 && line[len - 1] == ':') {
            line[len - 1] = '\0';
            labels[current_label_index].title = strdup(line);
            labels[current_label_index].mem_pos = current_instruction_index;
            current_label_index++;
            continue;
        }

        char* instruction_name = strtok(line, " \t");
        char* operand_name = strtok(NULL, " \t");

        if (!operand_name) {
            printf("Erro: Instrução sem operando: %s\n", instruction_name);
            for (int j = 0; j < current_label_index; j++) {
                free(labels[j].title);
            }
            free(labels);
            free(instructions);
            free(task_code_section);
            return NULL;
        }

        char* intr_name_copy = strdup(instruction_name);
        intr_name_copy = to_lowercase(intr_name_copy);
        int opcode = get_op_code_from_instruction_name(intr_name_copy);
        free(intr_name_copy);

        if (opcode < 0) {
            printf("Erro: Instrução desconhecida: %s\n", instruction_name);
            for (int j = 0; j < current_label_index; j++) {
                free(labels[j].title);
            }
            free(labels);
            free(instructions);
            free(task_code_section);
            return NULL;
        }
        
        memset(&instructions[current_instruction_index], 0, sizeof(Instruction));

        instructions[current_instruction_index].fn = get_instruction_function(opcode);
        instructions[current_instruction_index].operand = strdup(operand_name);

        if (operand_name[0] == '#') {
            instructions[current_instruction_index].type    = PARAM_INT;
        } else if (opcode == BRANY || opcode == BRPOS || opcode == BRZERO || opcode == BRNEG) {
            instructions[current_instruction_index].type    = PARAM_LABEL;
        } else {
            instructions[current_instruction_index].type    = PARAM_STRING;
        }

        current_instruction_index++;
    }

    printf("\n===== Instruções Parseadas =====\n");
    for (int i = 0; i < instruction_count; i++) {
        printf("Instrução %d:\n", i);
        printf("  Opcode: %d\n", get_opcode_from_function(instructions[i].fn));
        printf("  Operando: %s\n", instructions[i].operand);
        
        printf("  Tipo: ");
        switch(instructions[i].type) {
            case PARAM_INT:
                printf("Inteiro (#%s)\n", instructions[i].operand[0] == '#' ? instructions[i].operand + 1 : instructions[i].operand);
                break;
            case PARAM_LABEL:
                printf("Label (rótulo)\n");
                break;
            case PARAM_STRING:
                printf("String (variável)\n");
                break;
            default:
                printf("Desconhecido\n");
        }
        printf("\n");
    }
    printf("==============================\n\n");

    printf("\n===== Labels Parseados =====\n");
    for (int i = 0; i < label_count; i++) {
        printf("Label %d: %s (posição: %zu)\n", 
               i, labels[i].title, labels[i].mem_pos);
    }
    printf("===========================\n\n");

    task_code_section->instructions = instructions;
    task_code_section->labels = labels;

    return task_code_section;
}