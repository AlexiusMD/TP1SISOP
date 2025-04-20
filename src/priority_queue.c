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

PriorityQueue* priority_queue_init(int initial_capacity) {
    PriorityQueue* queue = malloc(sizeof(PriorityQueue));
    if (!queue) return NULL;

    queue->heap = malloc(sizeof(TaskControlBlock*) * initial_capacity);
    if (!queue->heap) {
        free(queue);
        return NULL;
    }

    queue->size = 0;
    queue->capacity = initial_capacity;
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
    while (index > 0 && queue->heap[parent(index)]->absolute_deadline > queue->heap[index]->absolute_deadline) {
        swap(&queue->heap[index], &queue->heap[parent(index)]);
        index = parent(index);
    }
}

void heapify_down(PriorityQueue* queue, int index) {
    int smallest = index;
    int l = left(index);
    int r = right(index);

    if (l < queue->size && queue->heap[l]->absolute_deadline < queue->heap[smallest]->absolute_deadline)
        smallest = l;
    if (r < queue->size && queue->heap[r]->absolute_deadline < queue->heap[smallest]->absolute_deadline)
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

void dequeue(PriorityQueue* queue) {
    if (queue->size == 0) return;

    queue->heap[0] = queue->heap[queue->size - 1];
    queue->size--;
    heapify_down(queue, 0);
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
