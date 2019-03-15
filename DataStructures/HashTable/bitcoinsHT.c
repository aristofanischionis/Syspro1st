#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/Tree.h"

static bitcoin DELETED_BITCOIN = {-1, NULL};

int HT_INITIAL_BASE_SIZE_BTC = 200; // a default number for the base HT


static BitcoinHT* newSizeBTC(const int baseSize){
    BitcoinHT* ht = malloc(sizeof(BitcoinHT));
    ht->baseSize = baseSize;
    ht->size = baseSize;
    ht->count = 0;
    ht->nodes = malloc((size_t)ht->size * sizeof(bitcoin*));
    int i;
    for(i=0;i<baseSize;i++){
        // ht->nodes[i] = malloc(sizeof(bitcoin));
        ht->nodes[i] = NULL;
    }
    return ht;
}

BitcoinHT* newBTC(const int size){
    HT_INITIAL_BASE_SIZE_BTC = size;
    return newSizeBTC(HT_INITIAL_BASE_SIZE_BTC);
}

static void delNodeBTC(bitcoin* i){
    if(i->btcTree != NULL){
        destroyTree(i->btcTree);
    }
    free(i);
}

void delHTBTC(BitcoinHT* ht) {
    for (int i = 0; i < ht->size; i++) {
        bitcoin* item = ht->nodes[i];
        if (item != NULL) {
            delNodeBTC(item);
        }
    }
    free(ht->nodes);
    free(ht);
}
// hash function got from the web
static int getHashBTC(int key, const int size, const int attempt) {
    unsigned long x = (unsigned long) key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    int temp = (int)(x % size);
    return ((temp + attempt) % size);
}

void insertBTC(BitcoinHT* ht, bitcoin* item) {
    if(item == NULL){
        printf("bitcoin is NULL\n");
        return;
    } 
    int index = getHashBTC(item->_bitcoinID, ht->size, 0);
    bitcoin* curItem = NULL;
    curItem = ht->nodes[index];
    
    int i = 1;
    while ( curItem != NULL ){
        if (curItem->_bitcoinID == item->_bitcoinID) {
            // if an _id is given that it has already been given
            printf("Error this bitcoin id already exists in the Hashtable\n");
            exit(1);
        }
        index = getHashBTC(item->_bitcoinID, ht->size, i);
        curItem = ht->nodes[index];
        i++;
    }
    // ht->nodes[index] = item;
    ht->nodes[index] = malloc(sizeof(bitcoin));
    memcpy(ht->nodes[index], item, sizeof(bitcoin));
    ht->count++; // bitcoin inserted
    return;
}

bitcoin* searchBTC(BitcoinHT* ht, int _id) {
    int index = getHashBTC(_id, ht->size, 0);
    bitcoin* item = ht->nodes[index];
    int i = 1;
    while (item != NULL) {
        // printf("%s---searching---%s,---->%d\n",item->_id,_id, i);
        // if (item != &DELETED_BITCOIN) { 
            if (item->_bitcoinID == _id) {
                // printf("search to %s kai brhka %s as epistrepsw %d \n",_id, item->_id, index);
                return item;
            }
        // }
        index = getHashBTC(_id, ht->size, i);
        item = ht->nodes[index];
        // if(item == NULL) printf("item is null %d\n", index);
        i++;
    } 
    return NULL;
}

// delete an item with HashTable resizeBTC if needed
void deleteBTC(BitcoinHT* ht, int _id) {
    int index = getHashBTC(_id, ht->size, 0);
    bitcoin* item = ht->nodes[index];
    int i = 1;
    while (item != NULL) {
        if (item != &DELETED_BITCOIN) {
            if (item->_bitcoinID == _id){
                delNodeBTC(item);
                ht->nodes[index] = &DELETED_BITCOIN;
            }
        }
        index = getHashBTC(_id, ht->size, i);
        item = ht->nodes[index];
        i++;
    } 
    ht->count--;
}

void printBTC(BitcoinHT* ht){
    int i =0 ;
    while(i < ht->baseSize){
        if(ht->nodes[i] != NULL){
            printf("This is the bitcoin --> |%d|\n", ht->nodes[i]->_bitcoinID);
            printTree(*(ht->nodes[i]->btcTree));
        }
        i++;
    }
}
