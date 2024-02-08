#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *g_head = NULL;

struct node *executed = NULL;

// comparison function to determine lexicographical order
bool comesBefore(char *a, char *b)
{
    return strcmp(a, b) < 0;
}

// pick the next task to run based on lexicographical order
Task *pickNextTask()
{
    if (!g_head)
    {
        return NULL;
    }

    struct node *temp = g_head;
    Task *highestPriority = temp->task;

    // find tasks with highest priority
    while (temp != NULL)
    {
        if (temp->task->priority > highestPriority->priority)
        {
            highestPriority = temp->task;
        }
        else if (temp->task->priority == highestPriority->priority && comesBefore(temp->task->name, highestPriority->name))
        {
            // If two tasks have the same priorty, choose based on lexicographical order
            highestPriority = temp->task;
        }
        temp = temp->next;
    }

    delete (&g_head, highestPriority);
    return highestPriority;
}

// add task to the list
void add(char *name, int priority, int burst)
{
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->execution = 0;
    insert(&g_head, newTask);
}
// if remaining burst time is greater than the quantum return the quantum
// else return burst time
int getBurst(Task *task)
{
    if (task->burst - task->execution > 10)
    {
        return 10;
    }
    else
    {
        return task->burst - task->execution;
    }
}

void schedule()
{
    // Keep track of current system time
    int currentTime = 0;

    // schedule as long as there are tasks in the list
    do
    {
        while (g_head)
        {
            // Select the next task to be executed based on the scheduling criteria
            Task *task = pickNextTask();

            int burst = getBurst(task);
            // run the task for the entire burst
            run(task, burst);
            task->execution += burst;
            if (task->execution < task->burst)
            {
                insert(&executed, task);
            }
            else
            {
                free(task);
            }
            // update the current time with the burst time
            currentTime += burst;
            printf("\tTime is now: %d\n", currentTime);
        }
        // move executed tasks back to ghead for next round
        g_head = executed;
        executed = NULL;
        // continue if there are tasks left
    } while (executed || g_head);
}