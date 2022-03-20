#include <stdlib.h> 
#include <stdio.h>      //Null
#include <stdbool.h>    //Boolean
#include <limits.h>     //Int_Max
#include "schedulers.h"
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
    int quantum = 0;
    int priorityIndex = 1;
    while(head != NULL){
        struct node* np = head;  
        if(np->task->priority == priorityIndex){
            run(np->task, np->task->burst);
            quantum += np->task->burst;
            delete(&head, np->task);
            printf("\tTime is now %d\n", quantum);
        }
        while(np->next != NULL){
            np = np->next;
            if(np->task->priority == priorityIndex){
                run(np->task, np->task->burst);
                quantum += np->task->burst;
                delete(&head, np->task);
                printf("\tTime is now %d\n", quantum);
            }  
        }
        priorityIndex++;
    }
};