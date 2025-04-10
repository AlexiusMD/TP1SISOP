#include "../includes/asm_wrapper.h"
#include "../includes/asm_funcs.h"
#include "../includes/asm_compiler.h"
#include "../includes/task.h"

void increment_wrapper(TaskControlBlock* tcb, void* operand) {
    increment(tcb, *((int*) operand));
}
void decrement_wrapper(TaskControlBlock* tcb, void* operand) {
    decrement(tcb, *((int*) operand));
}
void multiply_wrapper (TaskControlBlock* tcb, void* operand) {
    multiply(tcb, *((int*) operand));
}
void divide_wrapper   (TaskControlBlock* tcb, void* operand) {
    divide(tcb, *((int*) operand));
}
void load_wrapper     (TaskControlBlock* tcb, void* operand) {
    load(tcb, (char*) operand);
}
void store_wrapper    (TaskControlBlock* tcb, void* operand) {
    store(tcb, (char*) operand);
}
void brany_wrapper(TaskControlBlock* tcb, void* operand) {
    brany(tcb, (Label*) operand);
}
void brpos_wrapper(TaskControlBlock* tcb, void* operand) {
    brpos(tcb, (Label*)operand);
}
void brzero_wrapper(TaskControlBlock* tcb, void* operand) {
    brzero(tcb, (Label*)operand);
}
void brneg_wrapper(TaskControlBlock* tcb, void* operand) {
    brneg(tcb, (Label*)operand);
}
void syscall_wrapper(TaskControlBlock* tcb, void* operand) {
    syscall(tcb, *((int*) operand));
}