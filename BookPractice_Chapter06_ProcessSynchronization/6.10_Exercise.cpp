
#include <cstdlib>
#include <pthread.h>
using namespace std;



typedef struct node { 
  int data; 
  struct node *next;
} Node; 

Node *top; // Pointer to top of stack

// ---> Push() function that takes Node.data as an argument
void push(int item) { 
  Node *old_node;       //Create Old Node Pointer
  Node *new_node;       //Create New Node Pointer
  new_node = (node*)malloc(sizeof(Node));   //Memory Allocation the size of Node and point new node 
                                            //      at that memory location
  new_node->data = item;    //Assign the passed in value to the new nodes .data attribute
  do {
   old_node = top;          //Point the Old Node pointers at the top pointer

   new_node->next = old_node;       //Point the New Nodes->next pointer to the old node
  } while (compare_and_swap(top, old_node, new_node) != old_node); 
}

int pop() { 
  Node *old_node;   //Create Old Node Pointer
  Node *new_node;   //Create New Node Pointer
  do {
    old_node = top; //Point Old node pointer at the top


    
    if (old_node == NULL){      //If the Old Node pointer isn't pointing at nothing
        return NULL;            //return NULL
    }
    new_node = old_node->next;  //If the Old pointer is pointing at something, Assign the new node pointer
                                //      to the old node next

  } while (compare_and_swap(top, old_node, new_node) != old_node); 
  

  return old_node->data;
