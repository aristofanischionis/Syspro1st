#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/LinkedLists.h"
#include "../../HeaderFiles/Structs.h"

int init(LinkedList *list, int size, freeFunction fn)
{
    if (size <= 0)
        return ERROR;
    list->length = 0;
    list->dataSize = size;
    list->head = NULL;
    list->tail = NULL;
    list->freeFn = fn;
    return SUCCESS;
}

int destroy(LinkedList *list)
{
    listNode *cur;
    while (list->head != NULL)
    {
        cur = list->head;
        list->head = cur->next;

        if (list->freeFn)
        {
            list->freeFn(cur->data);
        }

        free(cur->data);
        free(cur);
    }
    return SUCCESS;
}

int insertBEG(LinkedList *list, void *newData)
{
    // Allocate memory for node
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = malloc(list->dataSize);
    // copy data to new node
    memcpy(newNode->data, newData, list->dataSize);
    if (newNode->data == NULL)
        return ERROR;
    // make it first in the list and point next to current head
    newNode->next = list->head;
    list->head = newNode;
    // if it is the first element
    if (list->tail == NULL)
    {
        list->tail = list->head;
    }
    // one more item in our list
    list->length++;
    return SUCCESS;
}

int insertEND(LinkedList *list, void *newData)
{
    // Allocate memory for node
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = malloc(list->dataSize);
    newNode->next = NULL;

    // copy data to new node
    memcpy(newNode->data, newData, list->dataSize);
    if (newNode->data == NULL)
        return ERROR;

    if (list->length == 0)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        // current tail's next will be the new node
        list->tail->next = newNode;
        // and the tail pointer will point in the previous's next
        // it's the same as list->tail = list->tail->next
        list->tail = newNode;
    }

    list->length++;
    return SUCCESS;
}

int doForAll(LinkedList *list, repeator rep)
{
    if (rep == NULL)
        return ERROR;
    int result = 1;

    listNode *node = list->head;

    while (node != NULL && result)
    {
        result = rep(node->data);
        node = node->next;
    }
    return SUCCESS;
}

int listHead(LinkedList *list, void *item, int removal)
{
    if (list->head == NULL) return ERROR;

    listNode *node = list->head;
    memcpy(item, node->data, list->dataSize);

    if (removal)
    {
        list->head = node->next;
        list->length --;

        free(node->data);
        free(node);
    }
    return SUCCESS;
}

int listTail(LinkedList* list, void *item)
{
    if(list->tail == NULL) return ERROR;

    listNode *node = list->tail;
    memcpy(item, node->data, list->dataSize);
    
    return SUCCESS;
}

void printList(LinkedList *list, void (*fptr)(void *))
{
    listNode *node = list->head;
    while (node != NULL)
    {
        (*fptr)(node->data);
        node = node->next;
    }
}

// Function to print an trx
void printTRXobj(void *t)
{
    trxinLL* tr;
    tr = (trxinLL*)t;
    int id = tr->trx->_trxID;
    
    printf("TRX id is:  %d\n", id);

    printf(" wallet in trx is: %s\n ", tr->walletinTRX->_walletID);
}
