#include "../includes/asm_compiler.h"

int main(int argc, char* argv[]) {
    const char* filepath = "programs/prog2.txt";
    const char* filepath2 = "programs/prog1.txt";
    create_task(filepath, 0, 20);
    create_task(filepath2, 5, 20);
    return 0;
}