#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

typedef struct TaskControlBlock TaskControlBlock;

typedef int (*CompareFn)(TaskControlBlock*, TaskControlBlock*);

typedef struct PriorityQueue {
    TaskControlBlock** heap;
    int size;
    int capacity;
    CompareFn compare;
} PriorityQueue;

/*
    Queue functions
*/

void enqueue(TaskControlBlock* tcb, PriorityQueue* queue);
TaskControlBlock* dequeue(PriorityQueue* queue);
PriorityQueue* priority_queue_init(int initial_capacity, CompareFn compare);
void priority_queue_free(PriorityQueue* queue);
void resize_queue(PriorityQueue* queue);
void print_queue(PriorityQueue* queue);
TaskControlBlock* peek(PriorityQueue* queue);

int compare_by_deadline(TaskControlBlock* a, TaskControlBlock* b);
int compare_by_arrival(TaskControlBlock* a, TaskControlBlock* b);

/*
    Heap helpers
*/

void swap(TaskControlBlock** a, TaskControlBlock** b);
int parent(int i);
int left(int i);
int right(int i);
void heapify_down(PriorityQueue* queue, int index);
void heapify_up(PriorityQueue* queue, int index);

#endif
