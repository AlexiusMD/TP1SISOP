#include "../includes/priority_queue.h"
#include "../includes/task.h"
#include <stdlib.h>
#include <stdio.h>

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void swap(TaskControlBlock** a, TaskControlBlock** b) {
    TaskControlBlock* temp = *a;
    *a = *b;
    *b = temp;
}

int compare_by_deadline(TaskControlBlock* a, TaskControlBlock* b) {
    return a->absolute_deadline - b->absolute_deadline;
}

int compare_by_arrival(TaskControlBlock* a, TaskControlBlock* b) {
    return a->arrival_time - b->arrival_time;
}

PriorityQueue* priority_queue_init(int initial_capacity, CompareFn compare) {
    PriorityQueue* queue = malloc(sizeof(PriorityQueue));
    if (!queue) return NULL;

    queue->heap = malloc(sizeof(TaskControlBlock*) * initial_capacity);
    if (!queue->heap) {
        free(queue);
        return NULL;
    }

    queue->size = 0;
    queue->capacity = initial_capacity;
    queue->compare = compare;
    return queue;
}

void resize_queue(PriorityQueue* queue) {
    queue->capacity *= 2;
    queue->heap = realloc(queue->heap, sizeof(TaskControlBlock*) * queue->capacity);
    if (!queue->heap) {
        fprintf(stderr, "Erro: realloc falhou\n");
        exit(EXIT_FAILURE);
    }
}

void heapify_up(PriorityQueue* queue, int index) {
    while (index > 0 && queue->compare(queue->heap[index], queue->heap[parent(index)]) < 0) {
        swap(&queue->heap[index], &queue->heap[parent(index)]);
        index = parent(index);
    }
}

void heapify_down(PriorityQueue* queue, int index) {
    int smallest = index;
    int l = left(index);
    int r = right(index);

    if (l < queue->size && 
        queue->compare(queue->heap[l], queue->heap[smallest]) < 0)
        smallest = l;
    if (r < queue->size && 
        queue->compare(queue->heap[r], queue->heap[smallest]) < 0)
        smallest = r;

    if (smallest != index) {
        swap(&queue->heap[index], &queue->heap[smallest]);
        heapify_down(queue, smallest);
    }
}

void enqueue(TaskControlBlock* tcb, PriorityQueue* queue) {
    if (queue->size >= queue->capacity)
        resize_queue(queue);

    queue->heap[queue->size] = tcb;
    heapify_up(queue, queue->size);
    queue->size++;
}

TaskControlBlock* dequeue(PriorityQueue* queue) {
    if (queue->size == 0) return NULL;

    TaskControlBlock* first_in_queue = queue->heap[0];

    queue->heap[0] = queue->heap[queue->size - 1];
    queue->size--;
    if (queue->size > 0) {
        heapify_down(queue, 0);
    }

    return first_in_queue;
}

void priority_queue_free(PriorityQueue* queue) {
    if (queue) {
        free(queue->heap);
        free(queue);
    }
}

void print_queue(PriorityQueue* queue) {
    for (int i = 0; i < queue->size; i++) {
        printf("Task PID: %zu, Deadline: %d\n", queue->heap[i]->pid, queue->heap[i]->absolute_deadline);
    }
}

TaskControlBlock* peek(PriorityQueue* queue) {
    if (queue == NULL || queue->size == 0) {
        return NULL;
    }

    return queue->heap[0];
}
