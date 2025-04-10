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
    int actual_instruction_count = 0;
    for(int i = 0; i < count; i++) {
        char* line = strip_whitespace(instructions_text[i]);
        if (line && strlen(line) > 0 && line[strlen(line) - 1] != ':') {
            actual_instruction_count++;
        }
    }

    Instruction* result = (Instruction*)malloc(actual_instruction_count * sizeof(Instruction));
    if (!result) {
        printf("Erro na alocação de memória - Array de Instruções\n");
        return NULL;
    }

    int current_instruction_index = 0;
    for(int i = 0; i < count; i++) {
        char* current_instruction = strip_whitespace(instructions_text[i]);

        size_t len = strlen(current_instruction);
        if (!current_instruction || len == 0) {
            continue;
        }
        
        if (len > 0 && current_instruction[len - 1] == ':') {
            current_instruction[len - 1] = '\0';
            printf("Rótulo encontrado: %s\n", current_instruction);
            continue;
        }

        char* instruction_name = strtok(current_instruction, " \t");
        char* operand_name = strtok(NULL, " \t");

        if (!operand_name) {
            printf("Erro: Instrução sem operando: %s\n", instruction_name);
            free(result);
            return NULL;
        }

        char* intr_name_copy = strdup(instruction_name);
        intr_name_copy = to_lowercase(intr_name_copy);
        int opcode = get_op_code_from_instruction_name(intr_name_copy);
        free(intr_name_copy);

        if (opcode < 0) {
            printf("Erro: Instrução desconhecida: %s\n", instruction_name);
            free(result);
            return NULL;
        }
        
        memset(&result[current_instruction_index], 0, sizeof(Instruction));

        result[current_instruction_index].fn = get_instruction_function(opcode);
        result[current_instruction_index].operand = strdup(operand_name);

        if (operand_name[0] == '#') {
            result[current_instruction_index].type    = PARAM_INT;
        } else if (opcode == BRANY || opcode == BRPOS || opcode == BRZERO || opcode == BRNEG) {
            result[current_instruction_index].type    = PARAM_LABEL;
        } else {
            result[current_instruction_index].type    = PARAM_STRING;
        }

        current_instruction_index++;
    }

    printf("\n===== Instruções Parseadas =====\n");
    for (int i = 0; i < actual_instruction_count; i++) {
        printf("Instrução %d:\n", i);
        printf("  Opcode: %d\n", get_opcode_from_function(result[i].fn));
        printf("  Operando: %s\n", result[i].operand);
        
        printf("  Tipo: ");
        switch(result[i].type) {
            case PARAM_INT:
                printf("Inteiro (#%s)\n", result[i].operand[0] == '#' ? result[i].operand + 1 : result[i].operand);
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
    
    return result;
}