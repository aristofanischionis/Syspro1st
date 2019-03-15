#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

int iterateBitcoins(void *data);

// new wallet HashTable very much alike with the bitcoin hash table
static walletHT* newSize(const int baseSize) {
    walletHT* ht = malloc(sizeof(walletHT));
    ht->baseSize = baseSize;
    ht->size = baseSize;
    ht->count = 0;
    ht->nodes = malloc((size_t)ht->size * sizeof(wallet*));
    int i;
    for(i=0;i<baseSize;i++){
        ht->nodes[i] = NULL;
    }
    return ht;
}

walletHT* new(const int size) {
    return newSize(size);
}

static void delNode(wallet* i) {
    free(i->_walletID);
    if(i->btcList != NULL){
        destroy(i->btcList);
    }
    
    free(i);
}

void delHT(walletHT* ht) {
    for (int i = 0; i < ht->size; i++) {
        wallet* item = ht->nodes[i];
        if (item != NULL) {
            delNode(item);
        }
    }
    free(ht->nodes);
    free(ht);
}

// hash function
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

static int getHash( const char* s, const int size, const int attempt) {
    int hashA = hash(s, size);
    int result = (hashA + attempt) % size;
    return result;
}

// insert a wallet in the hashtable
void insert(walletHT* ht, wallet* item) {
    if(item == NULL){
        printf("wallet is NULL\n");
        return;
    }
    int index = getHash(item->_walletID, ht->size, 0);
    wallet* curItem = ht->nodes[index];
    int i = 1;

    while ( curItem != NULL ) {
        if (!strcmp(curItem->_walletID, item->_walletID)) {
            // if the _id given already exists here
            printf("Error this wallet id already exists in the Hashtable\n");
            exit(1);
            
        }
        index = getHash(item->_walletID, ht->size, i);
        curItem = ht->nodes[index];
        i++;
    } // avoid collisions, trying to find the suitable index to place the wallet
    ht->nodes[index] = malloc(sizeof(wallet));
    memcpy(ht->nodes[index], item, sizeof(wallet));
    ht->count++; // wallet inserted
    return;
}

wallet* search(walletHT* ht, char* _id) {
    int index = getHash(_id, ht->size, 0);
    wallet* item = ht->nodes[index];
    int i = 1;
    while (item != NULL) {
        if (strcmp(item->_walletID, _id) == 0) {
            return item;
        }
        index = getHash(_id, ht->size, i);
        item = ht->nodes[index];
        i++;
    } 
    return NULL;
}

// just for me to test in everything is inserted correctly
void print(walletHT* ht){
    int i =0 ;
    while(i < ht->baseSize){
        if(ht->nodes[i] != NULL){
            printf("|%s|\n", ht->nodes[i]->_walletID);
            LinkedList *temp = ht->nodes[i]->btcList;
            doForAll(temp, iterateBitcoins); 
        }
        i++;
    }
}

int iterateBitcoins(void *data){
    userBitcoin* this;
    this = (userBitcoin*) data;
    int id = this->btc->_bitcoinID;
    int am = this->amount;
    printf("Id of this bitcoin %d and amount for this user %d\n", id, am);
    return 1;
}