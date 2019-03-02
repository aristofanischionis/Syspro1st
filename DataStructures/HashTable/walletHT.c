#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/Hashtables.h"
#include "../../HeaderFiles/LinkedLists.h"

static wallet DELETED_WALLET = {NULL, NULL, NULL};

int HT_INITIAL_BASE_SIZE = 200;

static void resizeUp(walletHT* );

static void resizeDown(walletHT* );

static walletHT* newSize(const int baseSize) {
    walletHT* ht = malloc(sizeof(walletHT));
    ht->baseSize = baseSize;
    ht->size = baseSize;
    ht->count = 0;
    ht->nodes = malloc((size_t)ht->size * sizeof(wallet*));
    return ht;
}

walletHT* new(const int size) {
    HT_INITIAL_BASE_SIZE = size;
    return newSize(HT_INITIAL_BASE_SIZE);
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
    int hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % m;
}
//avoiding collisions double hashing
static int getHash( const char* s, const int size, const int attempt) {
    const int hashA = hash(s, size);
    const int hashB = hash(s, size);
    return (hashA + (attempt * (hashB + 1))) % size;
}

void insert(walletHT* ht, wallet* item) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        resizeUp(ht);
    }

    // printf("id that is given after newWallet %s\n", item->_walletID);
    if(item == NULL) printf("wallet is NULL\n");
    int index = getHash(item->_walletID, ht->size, 0);
    wallet* curItem = ht->nodes[index];
    int i = 1;
    while ( curItem != NULL ) {
        if( curItem != &DELETED_WALLET ){
            if (strcmp(curItem->_walletID, item->_walletID) == 0) {
                // if an _id is given that it has already been given
                //update its content with the new value
                delNode(curItem);
                ht->nodes[index] = item;
                return;
            }
        }
        index = getHash(item->_walletID, ht->size, i);
        curItem = ht->nodes[index];
        i++;
    } // avoid collisions, trying to find the suitable index to place the wallet
    ht->nodes[index] = item;
    ht->count++; // wallet inserted
    return;
}

wallet* search(walletHT* ht, char* _id) {
    int index = getHash(_id, ht->size, 0);
    wallet* item = ht->nodes[index];
    int i = 1;
    while (item != NULL) {
        // printf("%s---searching---%s,---->%d\n",item->_id,_id, i);
        if (item != &DELETED_WALLET) { 
            if (strcmp(item->_walletID, _id) == 0) {
                // printf("search to %s kai brhka %s as epistrepsw %d \n",_id, item->_id, index);
                return item;
            }
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
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        resizeDown(ht);
    }
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

static void resize(walletHT* ht, const int baseSize) {
    if (baseSize < HT_INITIAL_BASE_SIZE) {
        return;
    }
    walletHT* newHT = newSize(baseSize);
    for (int i = 0; i < ht->size; i++) {
        wallet* item = ht->nodes[i];
        if (item != NULL && item != &DELETED_WALLET) {
            insert(newHT, item);
        }
    }

    ht->baseSize = newHT->baseSize;
    ht->count = newHT->count;

    // To delete newHT, we give it ht's size and nodes 
    const int tmp_size = ht->size;
    ht->size = newHT->size;
    newHT->size = tmp_size;

    wallet** tmp_items = ht->nodes;
    ht->nodes = newHT->nodes;
    newHT->nodes = tmp_items;

    delHT(newHT);
}

static void resizeUp(walletHT* ht) {
    const int new_size = ht->baseSize * 2;
    resize(ht, new_size);
}


static void resizeDown(walletHT* ht) {
    const int new_size = ht->baseSize / 2;
    resize(ht, new_size);
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

int iterateBitcoins(void *data) {
    userBitcoin* this;
    this = (userBitcoin*) data;
    int id = this->btc->_bitcoinID;
    int am = this->amount;
    printf("Id of this bitcoin %d and amount for this user %d\n", id, am);
    return 1;
}