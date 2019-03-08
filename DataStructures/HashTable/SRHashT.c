#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

void deleteBucket(void* t);

SRHashT* initSRHT(int h1, int numOfBucketNodes){
    SRHashT* ht;
    if(h1 < 0) return NULL;
    ht = (SRHashT*)malloc(sizeof(SRHashT)); // one ht
    ht->size = h1;
    ht->bucketNodesNum = numOfBucketNodes;
    ht->myBuckets = (LinkedList**)malloc(h1 * sizeof(LinkedList*)); // this is a dynamic array of h1 elements of type LL* bucket
    int i;
    for(i=0; i<h1; i++){
        ht->myBuckets[i] = init(sizeof(bucket*), deleteBucket);
        // ht->myBuckets[i] = NULL;
    }
    return ht;
}

void deleteBucketNode(bucketNode* this){
    destroy(this->headofList);
    free(this);
}

void deleteBucket(void* t){
    bucket* this = (bucket*) t;
    int i;
    for(i=0; i<this->size && this != NULL; i++){
        deleteBucketNode(this->array[i]);
    }
    free(this);
}

// hash function
static int hash(const char* str, const int m) {
    unsigned long hash = 5381;
    int c = (*str);
    while (c){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c = (*str++) ;
    }
    return hash % m;
}

static int getHash1( const char* s, const int size) {
    const int hashA = hash(s, size);
    return (hashA + 1) % size;
}

bucket* traverseLL(LinkedList* listofBuckets){
    listNode* h = listofBuckets->head;
    bucket* temp;
    while (h != NULL)
    {
        temp = (bucket*) h->data ;
        if(temp->count < temp->size){
            // this can fit more records in it
            return temp;
        }
        
        // this is just a full bucket
        h = h->next;
    }
    return NULL;
}
// search in a ll of bucket*
bucketNode* searchinLL(LinkedList* listofBuckets, char* _walletID){
    listNode* h = listofBuckets->head;
    bucket* temp;
    int i;
    while (h != NULL)
    {
        temp = (bucket*) h->data ;
        printf("this bucket has got %d items\n", temp->count);
        for(i=0; i<temp->count ; i++){
            if(!strcmp(temp->array[i]->walletID->_walletID, _walletID)){
                printf("Found the walletID! %s\n", temp->array[i]->walletID->_walletID);
                // result = temp->array[i];
                return temp->array[i];
            }
        }
        printf("I couldn't find the walletID %s in this bucket\n", _walletID);
        h = h->next;
    }
    printf("I wasn't able to find walletID %s in this LL\n", _walletID);
    return NULL;
}

int insertSRHT(SRHashT* ht, bucketNode* bkt, char* _id ){
    // hash sender
    if(bkt == NULL || ht == NULL || _id == NULL) return ERROR;

    int index = getHash1(_id, ht->size);
    // bucket* curItem = NULL;
    bucket* curItem;
    // go through the list and find the first not full bucket* of the list
    // linked list of bucket*
    curItem = traverseLL(ht->myBuckets[index]);

    if(curItem == NULL ){
        printf("I have to make a new bucket! \n");
         // then that means we have to make a new bucket in this list
        bucket* newBuck;
        // malloc the new
        newBuck = newBucket(ht->bucketNodesNum);
        // insert the bucknode
        insertNodeinBucket(newBuck, bkt);
        // insert bucket in list
        insertEND(ht->myBuckets[index], newBuck);
        return SUCCESS;
    } 

    // curItem = (bucket*) ht->myBuckets[index]->head->data;
    // if(curItem == NULL) {
    //     printf("something is really wrong\n");
    //     return ERROR;
    // }

    
    if(curItem->count == ht->bucketNodesNum){
        printf("I have to make a new bucket! \n");
        // then that means we have to make a new bucket in this list
        bucket* newBuck;
        // malloc the new
        newBuck = newBucket(ht->bucketNodesNum);
        // insert the bucknode
        insertNodeinBucket(newBuck, bkt);
        // insert bucket in list
        insertEND(ht->myBuckets[index], newBuck);
        return SUCCESS;
    }
    else if(curItem->count < curItem->size){
        // I can add one more bucketnode in this bucket
        insertNodeinBucket(curItem, bkt);
        return SUCCESS;
    }
    else {
        printf("Some weird magic happened here! \n");
        return ERROR;
    }
    // did that logic in insertNodeinBucket function
    // index uparxei
        // an uparxei sender
            // addsthn arxh trx
        // an oxi sender
            // koitaw an exei xwro to bucket
                // kai to bazw
                // alliws den to bazw
}

bucketNode* searchSRHT(SRHashT* ht, char* _id){
    if(ht == NULL || _id == NULL) return NULL;
    bucketNode* res;
    int index = getHash1(_id, ht->size);
    LinkedList* curItem = ht->myBuckets[index];

    res = searchinLL(curItem, _id);
    
    return res;
}

void deleteSRHT(SRHashT* ht){
    int i;
    for(i=0;i<ht->size;i++){
        destroy(ht->myBuckets[i]);
    }
    free(ht);
}

