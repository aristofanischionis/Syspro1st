#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/LinkedLists.h"
#include "../../HeaderFiles/Structs.h"

// implementation of a generic linked list
// got most of it from the web and then redifined it in order to fit with this Homework
// i must had implemented generic hashtable as well, would be so much easier afterall i think

// initializing the list
LinkedList* init(int size, freeFunction fn){
    LinkedList* list;
    list = malloc(sizeof(LinkedList));
    if (size <= 0)
        return NULL;
    list->length = 0;
    list->dataSize = size;
    list->head = NULL;
    list->tail = NULL;
    list->freeFn = fn;
    return list;
}

// destroying the list
int destroy(LinkedList *list){
    listNode *cur;
    while (list->head != NULL)
    {
        cur = list->head;
        list->head = cur->next;

        if (list->freeFn)
        {
            list->freeFn(cur->data);
        }

        // free(cur->data);
        free(cur);
    }
    return SUCCESS;
}

// insert in the beginning of the list
// i like this more because it's faster doesn't have to go through all of the list to find the end and insert there
int insertBEG(LinkedList *list, void *newData){
    if(list->dataSize == 0 || newData == NULL){
        printf("something went wrong here datasize of list == 0 or newdata is null\n");
        return ERROR;
    }
    // Allocate memory for node
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = malloc(sizeof(list->dataSize));
    // copy data to new node
    newNode->data = newData;
    
    if (newNode->data == NULL) return ERROR;
    // make it first in the list and point next to current head
    newNode->next = list->head;
    list->head = newNode;
    // if it is the first element
    if (list->tail == NULL){
        list->tail = list->head;
    }
    // one more item in our list
    list->length++;
    return SUCCESS;
}

//insert in the end
// it's very useful in breaking the bitcoins, i put them in the end of the list so they are not used so much and i don't have to 
// go through a lot of entries in the btc tree
int insertEND(LinkedList *list, void *newData){
    if(list->dataSize == 0 || newData == NULL){
        printf("something went wrong here datasize of list == 0 or newdata is null\n");
        return ERROR;
    }
    // Allocate memory for node
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = malloc(sizeof(list->dataSize));
    newNode->next = NULL;

    // copy data to new node
    newNode->data = newData;

    if (newNode->data == NULL) return ERROR;

    if (list->length == 0){
        list->head = newNode;
        list->tail = newNode;
    }
    else{
        // current tail's next will be the new node
        list->tail->next = newNode;
        // and the tail pointer will point in the previous's next
        // it's the same as list->tail = list->tail->next
        list->tail = newNode;
    }

    list->length++;
    return SUCCESS;
}

// can be used fo repetitive functions over list's elements
int doForAll(LinkedList *list, repeator rep){
    if (rep == NULL) return ERROR;
    int result = 1;

    listNode *node = list->head;

    while (node != NULL && result){
        result = rep(node->data);
        node = node->next;
    }
    return SUCCESS;
}
