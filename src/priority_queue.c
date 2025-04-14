#include "../includes/priority_queue.h"
#include "../includes/task.h"
#include <stdlib.h>
#include <stdio.h>

void enqueue(TaskControlBlock* tcb, PriorityQueue* queue) {
    if (queue->size == queue->capacity) {
        printf("Queue is full, cannot enqueue task.\n");
        return;
    }
    
    if (queue->size == 0){
        queue->queue[0] = tcb;
    }
    else {
        int i;
        for (i = queue->size - 1; (i >= 0 && queue->queue[i]->absolute_deadline < tcb->absolute_deadline); i--) {
            queue->queue[i + 1] = queue->queue[i];
        }
        queue->queue[i + 1] = tcb;
    }

    queue->size++;
}

void dequeue(TaskControlBlock* tcb, PriorityQueue* queue) {
    if (queue->size == 0) {
        printf("Queue is empty, cannot dequeue task.\n");
        return;
    }
    
    tcb = queue->queue[0];
    for (int i = 1; i < queue->size; i++) {
        queue->queue[i - 1] = queue->queue[i];
    }
    
    queue->size--;
}

void priority_queue_init(int initial_capacity) {
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    queue->size = 0;
    queue->capacity = initial_capacity;
}

void priority_queue_free(PriorityQueue* queue) {
    free(queue);
}