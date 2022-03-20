#include <stdlib.h> 
#include <stdio.h>      //Null
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
    int totalQuantum = 0;
    int burst = INT_MAX;
    
    struct task* minBurstPointer;
    struct node* np;
    
    while(head != NULL){
        np = head;
        while(np != NULL){
            if(np->task->burst < burst && np->task->burst != burst ){
                burst = np->task->burst;
                minBurstPointer = np->task;
            }
            np = np->next;
        }
        run(minBurstPointer, minBurstPointer->burst);
        delete(&head, minBurstPointer);
        totalQuantum += burst;
        printf("\tTime is now: %d\n", totalQuantum);
        burst = INT_MAX;
    }
};