#ifndef LINKEDLISTS_HEADER
#define LINKEDLISTS_HEADER
// generic implementation 
// source : https://pseudomuto.com/2013/05/implementing-a-generic-linked-list-in-c/
// a common function used to free malloc'd objects
typedef void (*freeFunction)(void *);
typedef int (*repeator)(void *);

struct listNode {
    void *data;
    struct listNode *next;
};
typedef struct listNode listNode;

struct LinkedList {
    // full length of the list
    int length;
    // size of each element
    int dataSize;
    // head of the list
    listNode* head;
    // I will use this to easily add items at the end of my list without traversing it everytime
    listNode* tail; 
    // function for freeing memory for the specific data type
    freeFunction freeFn;
};
typedef struct LinkedList LinkedList;



LinkedList* init(int size, freeFunction fn);
int destroy(LinkedList* list);
int insertBEG(LinkedList* list, void* newData);
int insertEND(LinkedList* list, void* newData);
int doForAll(LinkedList *list, repeator rep);
// int SkipNode(LinkedList *list, userBitcoin* ubtc);
int listHead(LinkedList *list, void *item, int removal);
int listTail(LinkedList* list, void *item);
void printList(LinkedList *node, void (*fptr)(void *));
void printTRXobj(void *t);

#endif