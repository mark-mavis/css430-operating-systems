#include <stdlib.h> 
#include <stdio.h>      //Null
#include <stdbool.h>
#include "schedulers.h"
#include <limits.h>
#include "cpu.h"
#include "task.h"
#include "list.h"

struct node* head = NULL;
void add(char *name, int priority, int burst){
    
    static int tid = 0;
    //Creates new Task
    Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = tid++;
    newTask->priority = priority;
    newTask->burst = burst;

    
    if(head == NULL){
        head = (struct node*)malloc(sizeof(struct node));
        head->task = newTask;
        head->next = NULL;        
    }else{
        //Pointer that walks down list
        struct node* nodePointer = head;
        while(nodePointer->next != NULL){
            nodePointer = nodePointer->next;
        }
        nodePointer->next = (struct node*)malloc(sizeof(struct node));
        nodePointer->next->task = newTask;
        nodePointer->next->next = NULL;
    }
};

void schedule(){  
    bool process_with_priority = true;
    bool process_with_roundrobin = false;
    int processCounter = 0;
    int quantum = 10;
    int totalQuantumRun = 0;
    int priorityIndex = INT_MAX;
    while(head != NULL){
        struct node* np = head;     //Iterator
        struct node* pnode;         //Priority Node
        while(np != NULL){         //While not Null
            if(np->task->priority < priorityIndex){     //Node Priority less than priorityIndex
                priorityIndex = np->task->priority;
                pnode = np;                
                processCounter = 1;
                process_with_priority = true;
                process_with_roundrobin = false;
            }else if(np->task->priority == priorityIndex){
                processCounter++;
                process_with_roundrobin = true;
                process_with_priority = false;
            }
            np = np->next;      //Move Node Pointer to next node
        }
        if(process_with_priority){
            run(pnode->task, pnode->task->burst);
            totalQuantumRun += pnode->task->burst;
            printf("\tTotal Time Now: %d\n", totalQuantumRun);
            delete(&head, pnode->task);
            processCounter = 0;
        }
        if(process_with_roundrobin){
            while(processCounter != 0){
                np = head;
                while(np != NULL){
                    if(np->task->priority == priorityIndex){
                        if(np->task->burst <= quantum){
                            run(np->task, np->task->burst);
                            totalQuantumRun += np->task->burst;
                            printf("\tTotal Time Now is: %d\n", totalQuantumRun);
                            delete(&head, np->task);
                            processCounter--;
                        }else{
                            run(np->task, quantum);
                            np->task->burst -= quantum;
                            totalQuantumRun += quantum;
                            printf("\tTotal Time Now is: %d\n", totalQuantumRun);
                        }
                    }
                    np=np->next;
                }
            }
        }
        process_with_priority = true;
        priorityIndex = INT_MAX;
    }
};

