#include "../includes/scheduler.h"
#include "../includes/task.h"
#include "../includes/priority_queue.h"
#include "../includes/asm_funcs.h"
#include "../includes/asm_compiler.h"
#include <stdlib.h>

void scheduler_init(PriorityQueue *queue, PriorityQueue *waiting_queue)
{

    while (queue->size > 0)
    {
        TaskControlBlock *tcb = queue->queue[0];
        dequeue(tcb, queue);

        tcb->state = RUNNING;
        tcb->program_counter++;
        tcb->deadline--;
        // Idk if the operand will throw an error
        tcb->instructions->fn(tcb, tcb->instructions->operand);

        if (tcb->instructions->type == SYSCALL)
        {
            tcb->remaining_blocking_time = rand() % 3 + 1;
            tcb->state = WAITING;
            enqueue(tcb, waiting_queue);
        }
    }
}