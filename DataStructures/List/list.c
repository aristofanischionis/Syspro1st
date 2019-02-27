#include <stdio.h> 
#include <stdlib.h> 
#include "../../HeaderFiles/LinkedLists.h"

// source: https://www.geeksforgeeks.org/generic-linked-list-in-c-2/

int insertintheEnd(LinkedList** head, void *newData, size_t dataSize) 
{ 
    // Allocate memory for node 
    LinkedList* newNode = (LinkedList*)malloc(sizeof(LinkedList));
    LinkedList* last = *head;
  
    newNode->data  = malloc(dataSize); 
  
    // Copy contents of newData to newly allocated memory. 
    int i; 
    for (i=0; i<dataSize; i++) 
        *(char *)(newNode->data + i) = *(char *)(newData + i); 
  
    newNode->next = NULL; 
    // If the Linked List is empty, then make the new node as head
    if (*head == NULL) { 
        *head = newNode;
        return 0; 
    } 
    // Else traverse till the last node
    while (last->next != NULL) last = last->next; 
  
    // Change the next of last node
    last->next = newNode;
    return 0; 
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