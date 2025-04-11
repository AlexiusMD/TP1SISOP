#include "../includes/asm_compiler.h"

int main(int argc, char* argv[]) {
    const char* filepath = "programs/prog2.txt";
    const char* filepath2 = "programs/prog1.txt";
    parse_program(filepath);
    parse_program(filepath2);
    return 0;
}