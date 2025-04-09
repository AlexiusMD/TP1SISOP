#include "scheduler.h"
#include "task.h"
#include "priority_queue.h"
#include "asm_funcs.h"
#include "asm_compiler.h"
#include <stdlib.h>


void scheduler_init(PriorityQueue* queue, PriorityQueue* waiting_queue) {

    while (queue->size > 0){
        TaskControlBlock* tcb = queue->queue[0];
        dequeue(tcb, queue);
        
        tcb->state = RUNNING;
        tcb->program_counter++;
        tcb->deadline--;
        // Idk if the operand will throw an error
        tcb->instructions->fn(tcb, tcb->instructions->operand);
    }
}