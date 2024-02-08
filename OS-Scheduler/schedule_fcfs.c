#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"


struct node *g_head = NULL; // global variable for the task list

// comparison function to determine lexicographical order
bool comesBefore(char *a, char *b) {
    return strcmp(a, b) < 0;
}

// pick the next task to run based on lexicographical order
Task *pickNextTask() {
    // if list is empty, nothing to do
    if (!g_head) {
        return NULL;
    }
    // Initialize a temporary pointer to traverse the list
    struct node *temp = g_head;

    // Start with the first task in the list as the best candidate so far
    Task *best_sofar = temp->task;

    // Traverse the list to find the task with the name that comes first in lexicographical order
    while (temp != NULL) {
        // If the current task's name comes before the best task's name in lexicographical order,
        // update the best task.
        if (comesBefore(temp->task->name, best_sofar->name)) {
            best_sofar = temp->task;
        }
        // Move to the next node in the list.
        temp = temp->next;
    }
    // delete the node from list, Task will get deleted later
    delete(&g_head, best_sofar);
    return best_sofar;
}
// add task to the list 
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name; 
    newTask->priority = priority;
    newTask-> burst = burst; 
    insert(&g_head, newTask); 
}

void schedule(){
    // Keep track of current system time 
    int currentTime = 0; 

    // schedule as long as there are tasks in the list 
    while(g_head){
        // Select the next task to be executed based on the scheduling criteria
        Task *task = pickNextTask(); 
        // run the task for the entire burst 
        run(task, task->burst);
        // update the current time with the burst time 
        currentTime += task->burst; 
        printf("\tTime is now: %d\n", currentTime);
        free(task); 
    }
}