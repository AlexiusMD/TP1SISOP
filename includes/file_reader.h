#ifndef FILE_READER_H
#define FILE_READER_H

#include "task.h"

char* read_program_file(const char* filename);
TaskControlBlock* parse_program(const char* filename);

#endif