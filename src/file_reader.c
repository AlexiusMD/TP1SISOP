#include "../includes/file_reader.h"
#include <stdlib.h>
#include <stdio.h>

char* read_program_file(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error reading from file.\n");
        printf("%s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(filesize + 1);
    if (!buffer) {
        printf("Erro during dynamic memory allocation\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';
    fclose(file);

    return buffer;
}