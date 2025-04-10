#ifndef FILE_READER_H
#define FILE_READER_H

typedef struct TaskControlBlock TaskControlBlock;

char* read_program_file(const char* filename);
TaskControlBlock* parse_program(const char* filename);

#endif