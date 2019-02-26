#include <stdio.h> 
#include <stdlib.h> 
#include "../../HeaderFiles/Structs.h"

// source: https://www.geeksforgeeks.org/generic-linked-list-in-c-2/

void push(LinkedList** head_ref, void *new_data, size_t data_size) 
{ 
    // Allocate memory for node 
    LinkedList* new_node = (LinkedList*)malloc(sizeof(LinkedList)); 
  
    new_node->data  = malloc(data_size); 
    new_node->next = (*head_ref); 
  
    // Copy contents of new_data to newly allocated memory. 
    int i; 
    for (i=0; i<data_size; i++) 
        *(char *)(new_node->data + i) = *(char *)(new_data + i); 
  
    // Change head pointer as new node is added at the beginning 
    (*head_ref) = new_node; 
} 

void printList(LinkedList *node, void (*fptr)(void *)) 
{ 
    while (node != NULL) 
    { 
        (*fptr)(node->data); 
        node = node->next; 
    } 
} 

// // Function to print an integer 
// void printInt(void *n) 
// { 
//    printf(" %d", *(int *)n); 
// } 
  
// // Function to print a float 
// void printFloat(void *f) 
// { 
//    printf(" %f", *(float *)f); 
// } 