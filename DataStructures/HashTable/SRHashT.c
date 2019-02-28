#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/Hashtables.h"
#include "../../HeaderFiles/LinkedList.h"

int init(SenderHT** ht, int h1){
    if(h1 < 0) return ERROR;
    ht = (SenderHT**)malloc(h1 * sizeof(SenderHT*));
    ht->size = h1;
    ht->count = 0;
    int i;
    for(i=0; i< h1; i++){
        ht->myBuckets[i] = NULL;
    }
}

int newBucket(SenderHT* ht, int index, int size ){
    if(ht == NULL) return ERROR;
    // malloc a bucket
    bucket* curBuck;
    curBuck = malloc(sizeof(bucket));
    // init the bucket
    curBuck->size = size;
    curBuck->array = (bucketNode*)malloc(size* sizeof(bucketNode));
    // check if this index has a list
    if(ht->myBuckets[index] == NULL){
        // this index doesn't have a list of buckets yet
        // so create it
        ht->myBuckets[index] = (LinkedList*)malloc(sizeof(LinkedList));
        insertEND(ht->myBuckets[index], curBucket);
    }
    else {
        insertEND(ht->myBuckets[index], curBucket);
    }
}

int insertBucket(){

}


int insertHT(SenderHT* ht, bucketNode* bkt, char* senderID ){

    // hash sender

    index den den uparxei
    newBucket();
    insertBucket(bkt); 
    

    // index uparxei
        // an uparxei sender
            // addsthn arxh trx
        // an oxi sender
            // koitaw an exei xwro to bucket
                // kai to bazw
                // alliws den to bazw

}

