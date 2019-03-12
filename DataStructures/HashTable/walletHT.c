#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

static wallet DELETED_WALLET = {NULL, NULL, -1, -1, -1};

int HT_INITIAL_BASE_SIZE_WAL = 200;

int iterateBitcoins(void *data);

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
    HT_INITIAL_BASE_SIZE_WAL = size;
    return newSize(HT_INITIAL_BASE_SIZE_WAL);
}

static void delNode(wallet* i) {
    destroy(i->btcList);
    free(i);
}

void delHT(walletHT* ht) {
    for (int i = 0; i < ht->size; i++) {
        wallet* item = ht->nodes[i];
        if (item != NULL && item != &DELETED_WALLET && !strcmp(item->_walletID,"")) {
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
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c = (*str++) ;
    }
    return hash % m;
}

static int getHash( const char* s, const int size, const int attempt) {
    int hashA = hash(s, size);
    // const int hashB = hash(s, size);
    int result = (hashA + attempt) % size;
    return result;
}

void insert(walletHT* ht, wallet* item) {
    if(item == NULL) printf("wallet is NULL\n");
    int index = getHash(item->_walletID, ht->size, 0);
    
    printf("index is %d \n", index);
    wallet* curItem = ht->nodes[index];
    // printf("id that is given after newWallet %s\n", ht->nodes[index]);
    int i = 1;
    while ( curItem != NULL ) {
        // if( curItem != &DELETED_WALLET ){
        if (strcmp(curItem->_walletID, item->_walletID) == 0) {
            // if an _id is given that it has already been given
            printf("Error this wallet id already exists in the Hashtable\n");
            exit(1);
            // delNode(curItem);
            // // ht->nodes[index] = item;
            // ht->nodes[index] = malloc(sizeof(wallet));
            // memcpy(ht->nodes[index], item, sizeof(wallet));
            // return;
        }
        // }
        index = getHash(item->_walletID, ht->size, i);
        curItem = ht->nodes[index];
        i++;
    } // avoid collisions, trying to find the suitable index to place the wallet
    // ht->nodes[index] = item;
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
        // printf("%s---searching---%s,---->%d\n",item->_id,_id, i);
        if (strcmp(item->_walletID, _id) == 0) {
            return item;
        }
        index = getHash(_id, ht->size, i);
        item = ht->nodes[index];
        // if(item == NULL) printf("item is null %d\n", index);
        i++;
    } 
    return NULL;
}
// delete an item with HashTable resize if needed

void delete(walletHT* ht, char* _id) {
    int index = getHash(_id, ht->size, 0);
    wallet* item = ht->nodes[index];
    int i = 1;
    while (item != NULL) {
        if (item != &DELETED_WALLET) {
            if (strcmp(item->_walletID, _id) == 0) {
                delNode(item);
                ht->nodes[index] = &DELETED_WALLET;
            }
        }
        index = getHash(_id, ht->size, i);
        item = ht->nodes[index];
        i++;
    } 
    ht->count--;
}

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