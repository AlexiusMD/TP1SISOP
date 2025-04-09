#include "../includes/asm_compiler.h"

int main(int argc, char* argv[]) {
    const char* filepath = "programs/prog2.txt";
    parse_program(filepath);
    return 0;
}