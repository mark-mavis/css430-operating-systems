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

int minVal(int burst, int slice){
    if(burst < slice){
        return burst;
    }else{
        return slice;
    }
}
void schedule(){  
    int totalQuantum = 0;
    int runtime_quantum;
    int quantum_slice = 10;
    
    while(head != NULL){
        struct node* np = head;
        if(np->task->burst != 0){
            runtime_quantum = minVal(np->task->burst, quantum_slice);
            run(np->task, runtime_quantum);
            totalQuantum += runtime_quantum;
            printf("\tTime is now %d\n", totalQuantum);
            if(np->task->burst != 0){
                add(np->task->name, np->task->priority, (np->task->burst - runtime_quantum));
                delete(&head, np->task);
            }
        }else{
            delete(&head, np->task);
        }
    }
};