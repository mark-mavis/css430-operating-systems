#include <stdlib.h> 
#include <stdio.h>      //Null
#include "schedulers.h"
#include "cpu.h"
#include "task.h"
#include "list.h"


struct node* head = NULL;
void add(char *name, int priority, int burst){
    
    static int tid = 1;
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
        struct node* np = head;
        while(np->next != NULL){
            np = np->next;
        }
        np->next = (struct node*)malloc(sizeof(struct node));
        np->next->task = newTask;
        np->next->next = NULL;
    }
};

void schedule(){
    int quantum = 0;
    struct node* np = head;
    while(np != NULL){
        run(np->task, np->task->burst);
        quantum += np->task->burst;
        printf("\tTime is now: %d\n", quantum);
        np = np->next;
    }
};