#include "../includes/asm_compiler.h"
#include "../includes/scheduler.h"
#include "../includes/priority_queue.h"
#include "../includes/task.h"

#include <stdio.h>

void printMenuOptions();
void getArrivalAndPeriod(int* arrival_time, int* period);
void CLIMenu();
void select_task_to_run(char* filename, int arrival_time, int period, PriorityQueue* arriving_queue, PriorityQueue* ready_queue);

int main(int argc, char* argv[]) {
    CLIMenu();
    return 0;
}

void printMenuOptions() {
    printf("Escolha qual programa quer carregar:\n");
    printf("====================================\n");
    printf("1. Programa 1\n");
    printf("2. Programa 2 \n");
    printf("3. Iniciar execução\n");
}

void select_task_to_run(char* filename, int arrival_time, int period, PriorityQueue* arriving_queue, PriorityQueue* ready_queue) {
    TaskControlBlock* tcb = create_task(filename, arrival_time, period, -1);
    if(arrival_time > 0) {
        enqueue(tcb, arriving_queue);
    } else {
        tcb->state = READY;
        enqueue(tcb, ready_queue);
    }
}

void CLIMenu() {
    int choice = 0;
    int arrival_time = 0;
    int period = 0;
    PriorityQueue* ready_queue = priority_queue_init(10, compare_by_deadline);
    PriorityQueue* waiting_queue = priority_queue_init(10, compare_by_deadline);
    PriorityQueue* arriving_queue = priority_queue_init(10, compare_by_arrival);

    while(1) {
        printMenuOptions();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                getArrivalAndPeriod(&arrival_time, &period);
                select_task_to_run("programs/prog1.txt", arrival_time, period, arriving_queue, ready_queue);
                break;
            case 2:
                getArrivalAndPeriod(&arrival_time, &period);
                select_task_to_run("programs/prog2.txt", arrival_time, period, arriving_queue, ready_queue);
                break;
            case 3:
                scheduler_init(ready_queue, waiting_queue, arriving_queue);
                priority_queue_free(ready_queue);
                priority_queue_free(waiting_queue);
                priority_queue_free(arriving_queue);
                return;
            default:
                printf("\nEscolha uma opção válida do menu!\n\n");
                break;
        }
    }
}

void getArrivalAndPeriod(int* arrival_time, int* period) {
    do {
        printf("Escolha um tempo de chegada para a tarefa. [>= 0]\n");
        scanf("%d", arrival_time);
        if(*arrival_time < 0) printf("\nEscolha um valor válido para a chegada!\n\n");
    } while (*arrival_time < 0);

    do {
        printf("Escolha um período para a tarefa. [> 0]\n");
        scanf("%d", period);

        if(*period <= 0) printf("\nEscolha um valor válido para o período!\n\n");
    } while (*period <= 0);
}