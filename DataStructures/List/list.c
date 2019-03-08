#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/LinkedLists.h"
#include "../../HeaderFiles/Structs.h"

LinkedList* init(int size, freeFunction fn)
{
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
    if(list->dataSize == 0 || newData == NULL){
        printf("something went wrong here datasize of list == 0 or newdata is null\n");
        return ERROR;
    }
    // Allocate memory for node
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = malloc(sizeof(list->dataSize));
    // copy data to new node
    // memcpy(newNode->data, newData, sizeof(list->dataSize));
    newNode->data = newData;
    
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
    if(list->dataSize == 0 || newData == NULL){
        printf("something went wrong here datasize of list == 0 or newdata is null\n");
        return ERROR;
    }
    // Allocate memory for node
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    newNode->data = malloc(sizeof(list->dataSize));
    newNode->next = NULL;

    // copy data to new node
    // memcpy(newNode->data, newData, sizeof(list->dataSize));
    newNode->data = newData;

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

// int SkipNode(LinkedList *list, userBitcoin* ubtc){ // do not free the bitcoin

//     listNode* temp = list->head;
//     listNode* prev;
//     userBitcoin* this;
//     // If head node itself holds the key to be deleted 
//     if(temp != NULL ){
//         this = (userBitcoin*) temp->data;
//         if(this->btc->_bitcoinID == ubtc->btc->_bitcoinID){
//             list->head = temp->next;
//             return SUCCESS;
//         }
//     }

//     // search the node to be deleted
//     while(temp != NULL && (this->btc->_bitcoinID != ubtc->btc->_bitcoinID)){
//         prev = temp;
//         temp = temp->next;
//     }

//     // If key was not present in linked list 
//     if (temp == NULL) return ERROR; 
  
//     // Unlink the node from linked list 
//     prev->next = temp->next; 

//     return SUCCESS;
// }

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
    trxObject* tr;
    tr = (trxObject*)t;
    char* id = malloc(15);
    strcpy(id, tr->_trxID);
    
    printf("TRX id is:  %s\n", id);

    printf("receiver is: %s and sender is: %s\n ", tr->receiver->_walletID, tr->sender->_walletID);
    free(id);
}
