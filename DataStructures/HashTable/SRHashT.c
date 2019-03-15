#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

void deleteBucket(void* t);
// this is the requested hashtable with the lists of buckets
// so I have a hashtable which contains an array of size h1, of pointers in LinkedLists of buckets
// each bucket contains a fixed array of pointers in bucketNodes, a size and a count to easily check if it is full
// each bucketNode contains a pointer to a wallet and a pointer in a linked list of his transactions

// initialize the Hashtable
SRHashT* initSRHT(int h1, int numOfBucketNodes){
    SRHashT* ht;
    if(h1 < 0) return NULL;
    ht = (SRHashT*)malloc(sizeof(SRHashT));
    ht->size = h1;
    ht->bucketNodesNum = numOfBucketNodes;
    // this is a dynamic array of h1 elements of type LL* bucket
    ht->myBuckets = (LinkedList**)malloc(h1 * sizeof(LinkedList*));
    int i;
    for(i=0; i<h1; i++){
        ht->myBuckets[i] = init(sizeof(bucket), deleteBucket);
    }
    return ht;
}

// deleting a bucketNode
void deleteBucketNode(bucketNode* this){
    if(this->headofList == NULL){
        free(this);
        return;
    }
    destroy(this->headofList);
    free(this);
}

// deleting a bucket
void deleteBucket(void* t){
    bucket* this = (bucket*) t;
    if(this == NULL) return;
    int i;
    for(i=0; i<this->size; i++){
        if(this->array[i] != NULL){
            deleteBucketNode(this->array[i]);
        }
    }
    free(this);
}

// deleting the hashtable
void deleteSRHT(SRHashT* ht){
    int i;
    for(i=0;i<ht->size;i++){
        destroy(ht->myBuckets[i]);
    }
    free(ht);
}

// hash function got from the web
// best for hashing a string
// using fast shifting operations and a good prime number
// proven to give best results in hashing
static int hash(const char* str, const int m) {
    unsigned long hash = 5381;
    int c = (*str);
    while (c){
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
        c = (*str++) ;
    }
    return hash % m;
}

static int getHash1( const char* s, const int size) {
    const int hashA = hash(s, size);
    return (hashA + 1) % size;
}

// traverse the list of buckets to find if there is any bucket already allocated
// with some space in it to put a new BucketNode
bucket* traverseLL(LinkedList* listofBuckets){
    if(listofBuckets == NULL){
        return NULL;
    }
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
// try to find the specific wallet id if already exists in a bucketnode of a bucket
// if exists i will put the transaction in his list
// else i will try to find another place to put the wallet id and his transactions
bucketNode* searchinLL(LinkedList* listofBuckets, char* _walletID){
    if(listofBuckets == NULL){
        return NULL;
    }
    listNode* h = listofBuckets->head;

    bucket* temp;
    int i;
    while (h != NULL)
    {
        temp = (bucket*) h->data ;
        // printf("this bucket has got %d items\n", temp->count);
        for(i=0; i<temp->count ; i++){
            if(!strcmp(temp->array[i]->walletID->_walletID, _walletID)){
                // printf("Found the walletID! %s\n", temp->array[i]->walletID->_walletID);
                // result = temp->array[i];
                return temp->array[i];
            }
        }
        // printf("I couldn't find the walletID %s in this bucket\n", _walletID);
        h = h->next;
    }
    // printf("I wasn't able to find walletID %s in this LL\n", _walletID);
    return NULL;
}

// take a bucketnode, an id and put the in the hashtable if they don't already exist there
int insertSRHT(SRHashT* ht, bucketNode* bkt, char* _id ){
    // hash sender
    if(bkt == NULL || ht == NULL || _id == NULL) return ERROR;

    int index = getHash1(_id, ht->size);
    // bucket* curItem = NULL;
    bucket* curItem;
    // check if in the index place there exists a bucket list
    if(ht->myBuckets[index] == NULL){
        ht->myBuckets[index] = newBucketList();
        curItem = NULL;
    }
    else{
        // if there exists find if there is any free place to put my bucketNode
        curItem = traverseLL(ht->myBuckets[index]);
    }
    // i couldn't find any free place so i have to allocate a new bucket
    if(curItem == NULL ){
        // then that means we have to make a new bucket in this list
        bucket* newBuck;
        // malloc the new
        newBuck = newBucket(ht->bucketNodesNum);
        // insert the bucknode
        insertNodeinBucket(newBuck, bkt);
        // insert bucket in list
        insertBEG(ht->myBuckets[index], newBuck);
        return SUCCESS;
    } 

    if(curItem->count == ht->bucketNodesNum){
        // then that means we have to make a new bucket in this list
        bucket* newBuck;
        // malloc the new
        newBuck = newBucket(ht->bucketNodesNum);
        // insert the bucknode
        insertNodeinBucket(newBuck, bkt);
        // insert bucket in list
        insertBEG(ht->myBuckets[index], newBuck);
        return SUCCESS;
    }
    else if(curItem->count < curItem->size){
        // I can add one more bucketnode in this bucket
        insertNodeinBucket(curItem, bkt);
        return SUCCESS;
    }
    else {
        // should never see that
        printf("Some weird magic happened here! \n");
        return ERROR;
    }
}

// return the bucketnode of this user id
bucketNode* searchSRHT(SRHashT* ht, char* _id){
    if(ht == NULL || _id == NULL) return NULL;
    bucketNode* res;
    int index = getHash1(_id, ht->size);
    LinkedList* curItem = ht->myBuckets[index];
    // find the requested id in this list of buckets*
    res = searchinLL(curItem, _id);
    return res;
}

// main function to be used outside of this file
// this is taking care of everything needed in order to place a trx in this hash table

void insertTransaction(walletHT* wHT, SRHashT* ht, char* _id, trxObject* trx){
    bucketNode* bkt1;
    bkt1 = searchSRHT(ht, _id);

    if(bkt1 == NULL){
        // first trx for sender
        // so
        bkt1 = newBucketNode(_id, wHT);
        insertBEG(bkt1->headofList, trx);
        insertSRHT(ht, bkt1, _id);
    }
    else{
        insertBEG(bkt1->headofList, trx);
    }
}