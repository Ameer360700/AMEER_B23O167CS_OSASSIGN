#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_SLICE 5
int completion_time = 0;
float total_turnaround = 0;

struct process {
    char process_name;
    int CPU_Burst_time;
    int arrival_time;
    int remaining_time;
    int completion_time;
    int queuenumber;
    struct process *next;
};

struct process *createnode(char process_name, int CPU_Burst_time) {
    struct process *newnode = (struct process *)malloc(sizeof(struct process));
    if (!newnode) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newnode->process_name = process_name;
    newnode->CPU_Burst_time = CPU_Burst_time;
    newnode->arrival_time = 0;
    newnode->remaining_time = CPU_Burst_time;
    newnode->completion_time = 0;
    newnode->queuenumber = 0;
    newnode->next = NULL;
    return newnode;
}

struct process *enqueue(struct process *head, struct process *p) {
    if (head == NULL) {
        return p;
    }
    struct process *temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = p;
    return head;
}

struct process *enqueue_priority(struct process *head, struct process *p) {
    if (head == NULL || p->remaining_time < head->remaining_time) {
        p->next = head;
        return p;
    }
    struct process *current = head;
    struct process *prev = NULL;
    while (current != NULL && current->remaining_time < p->remaining_time) {
        prev = current;
        current = current->next;
    }
    p->next = current;
    if (prev)
        prev->next = p;
    return head;
}

struct process *dequeue(struct process **head) {
    if (*head == NULL) {
        return NULL;
    }
    struct process *temp = *head;
    *head = (*head)->next;
    temp->next = NULL;
    return temp;
}

void RoundRobin(struct process **rrhead, struct process **fcfshead) {
    while (*rrhead != NULL) {
        struct process *current = dequeue(rrhead);
        if (current == NULL) {
            continue;
        }
        if (current->remaining_time > TIME_SLICE) {
            completion_time += TIME_SLICE;
            current->remaining_time -= TIME_SLICE;
            current->queuenumber = (current->queuenumber + 1) % 4;
            *fcfshead = enqueue(*fcfshead, current);
        } else {
            completion_time += current->remaining_time;
            int turnaround = completion_time - (current->arrival_time);
            total_turnaround += turnaround;
            printf("Turnaround time for Process %c = %d\n", current->process_name, turnaround);
            free(current);
        }
    }
}

void FCFS(struct process **fcfshead, struct process **priorityShead) {
    while (*fcfshead != NULL) {
        struct process *current = dequeue(fcfshead);
        if (current == NULL) {
            continue;
        }
        if (current->remaining_time > TIME_SLICE) {
            completion_time += TIME_SLICE;
            current->remaining_time -= TIME_SLICE;
            current->queuenumber = (current->queuenumber + 1) % 4;
            *priorityShead = enqueue_priority(*priorityShead, current);
        } else {
            completion_time += current->remaining_time;
            int turnaround = completion_time - (current->arrival_time);
            total_turnaround += turnaround;
            printf("Turnaround time for Process %c = %d\n", current->process_name, turnaround);
            free(current);
        }
    }
}

void PriorityS(struct process **priorityShead, struct process **sjfshead) {
    while (*priorityShead != NULL) {
        struct process *current = dequeue(priorityShead);
        if (current == NULL) {
            continue;
        }
        if (current->remaining_time > TIME_SLICE) {
            completion_time += TIME_SLICE;
            current->remaining_time -= TIME_SLICE;
            current->queuenumber = (current->queuenumber + 1) % 4;
            *sjfshead = enqueue_priority(*sjfshead, current);
        } else {
            completion_time += current->remaining_time;
            int turnaround = completion_time - (current->arrival_time);
            total_turnaround += turnaround;
            printf("Turnaround time for Process %c = %d\n", current->process_name, turnaround);
            free(current);
        }
    }
}

void SJFS(struct process **sjfshead, struct process **rrhead) {
    while (*sjfshead != NULL) {
        struct process *current = dequeue(sjfshead);
        if (current == NULL) {
            continue;
        }
        if (current->remaining_time > TIME_SLICE) {
            completion_time += TIME_SLICE;
            current->remaining_time -= TIME_SLICE;
            current->queuenumber = (current->queuenumber + 1) % 4;
            *rrhead = enqueue(*rrhead, current);
        } else {
            completion_time += current->remaining_time;
            int turnaround = completion_time - (current->arrival_time);
            total_turnaround += turnaround;
            printf("Turnaround time for Process %c = %d\n", current->process_name, turnaround);
            free(current);
        }
    }
}

int main() {
    int n = 5;
    struct process *rrhead = NULL;
    struct process *fcfshead = NULL;
    struct process *priorityShead = NULL;
    struct process *sjfshead = NULL;

    for (int i = 0; i < n; i++) {
        char process_name;
        int CPU_burst_time;
        printf("Enter process name: ");
        scanf(" %c", &process_name);
        printf("Enter CPU Burst time of process: ");
        scanf("%d", &CPU_burst_time);
        struct process *new_process_node = createnode(process_name, CPU_burst_time);
        rrhead = enqueue(rrhead, new_process_node);
    }

    while (rrhead || fcfshead || priorityShead || sjfshead) {
        if (rrhead) RoundRobin(&rrhead, &fcfshead);
        if (fcfshead) FCFS(&fcfshead, &priorityShead);
        if (priorityShead) PriorityS(&priorityShead, &sjfshead);
        if (sjfshead) SJFS(&sjfshead, &rrhead);
    }
    
    printf("Average turnaround time: %f\n", total_turnaround / n);
    return 0;
}