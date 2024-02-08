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
    if (!g_head) {
        return NULL;
    }

    struct node *temp = g_head;
    Task *shortestJob = temp->task;

    // find tasks with shortest burst time
    while (temp != NULL) {
        if (temp->task->burst < shortestJob->burst) {
            shortestJob = temp->task;
        } else if (temp->task->burst == shortestJob->burst && comesBefore(temp->task->name, shortestJob->name)) {
            // if two tasks have the same burst time, choose based on lexicographical order
            shortestJob = temp->task;
        }
        temp = temp->next;
    }

    delete(&g_head, shortestJob);
    return shortestJob;
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