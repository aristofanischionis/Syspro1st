#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/Tree.h"
#include "../../HeaderFiles/LinkedLists.h"

// in this file I put a lot of initializing functions stuff
// in most of them i allocate memory and return in a pointer of the specific type


// -------------------------------- CONSTRUCTORS --------------------------------//
//make a new wallet
wallet* newWallet(char* _walletID, LinkedList* btcList, int balance){
    wallet* wal;
    wal = malloc(sizeof(wallet));

    wal->_walletID = (char *)malloc(50);
    strcpy(wal->_walletID, _walletID);
    wal->balance = balance;
    wal->btcList = btcList;
    return wal;
}

userBitcoin* newUserBitcoin(int amount, bitcoin* b){
    userBitcoin* bcoin;
    bcoin = malloc(sizeof(userBitcoin));
    bcoin->amount = amount;
    bcoin->btc = b;
    return bcoin;
}

bitcoin* newBitcoin(int _bitcoinID, int btcVal, char* this){
    bitcoin* b;
    b = malloc(sizeof(bitcoin));
    b->_bitcoinID = _bitcoinID;
    b->btcTree = createTree(btcVal, this);
    return b;
}

btcNode* newBTCNode(char* walletID, int dollars, trxObject* txID){
    btcNode* b;
    b = malloc(sizeof(btcNode));

    b->dollars = dollars;
    b->walletID = malloc(50);
    strcpy(b->walletID, walletID);
    b->thisTrx = txID;
    return b;
}

trxObject* newTrxObj(char* sendID, char* recID, char* id, int val, struct tm* t){
    trxObject* trx;
    trx = malloc(sizeof(trxObject));

    trx->_trxID = malloc(15);
    strcpy(trx->_trxID, id);
    trx->value = val;
    trx->_time = t;
    trx->printed = NO;
    trx->sender = malloc(50);
    strcpy(trx->sender, sendID);
    trx->receiver = malloc(50);
    strcpy(trx->receiver, recID);

    return trx;
}

bucketNode* newBucketNode(char* wal, walletHT* ht){
    bucketNode* bkt;
    bkt = malloc(sizeof(bucketNode));
    wallet* res;
    res = search(ht, wal);
    if (res == NULL) return NULL;
    bkt->walletID = res;
    bkt->headofList = newTRXList();
    return bkt;
}

bucket* newBucket(int size){
    bucket* b;
    b = (bucket*) malloc(sizeof(bucket));
    b->size = size;
    b->count = 0;
    b->array = (bucketNode**)malloc(size* sizeof(bucketNode*));
    int i;
    for(i = 0; i<size ; i++){
        b->array[i] = (bucketNode*)malloc(sizeof(bucketNode));
        b->array[i] = NULL;
    }
    return b;
}

// -------------------------------- DESTRUCTORS --------------------------------//
// delete a bitcoin
void destroyBitcoin(void* data){
    bitcoin* b = (bitcoin*) data;
    if(b == NULL) return ;
    if(b->btcTree != NULL){
        destroyTree(b->btcTree);
    }
    // free(b);
}

void deleteTRX(trxObject* trx){
    if(trx == NULL) return;
    free(trx->receiver);
    free(trx->sender);
    free(trx->_time);
    free(trx->_trxID);
    free(trx);
}

void deleteBTCnode(btcNode* node){
    if(node == NULL) return;
    if(node->thisTrx == NULL) return;
    deleteTRX(node->thisTrx); 
    // printf("this trx is : %s\n", node->thisTrx->_trxID);  
    free(node->thisTrx);
    // free(node->walletID);
    // free(node);
}

void destroyUserBitcoin(void* data){
    if(data == NULL) return ;
    userBitcoin* d = (userBitcoin*)data;
    if(d->btc == NULL) return;
    destroyBitcoin(d->btc);
}

void freeString(void *data){
  free((char *)data);
}

void destroyBucket(void* data){
    if(data == NULL) return ;
    bucket* b = (bucket*)data;
    int i;
    for(i=0; i< b->count ; i++){
        if(b->array[i] == NULL) continue;
        if(b->array[i]->headofList == NULL) continue;
        destroy(b->array[i]->headofList);
        free(b->array[i]);
    }
    free(b);
}

// -------------------------------- MAKE LISTS --------------------------------//
LinkedList* newBtcList(){
    return init(sizeof(userBitcoin), destroyUserBitcoin);
}

LinkedList* newTRXList(){
    return init(sizeof(trxObject), NULL);
}

LinkedList* newBucketList(){
    return init(sizeof(bucket), destroyBucket);
}

// -------------------------------- OTHER FUNCTIONS --------------------------------//
// take a bucketnode and find where to put it in a bucket
// it has a trx in it
int insertNodeinBucket(bucket* b, bucketNode* bn){
    if(b->count == b->size){
        return ERROR;
    } 
    // check if the sender or receiver already exists in a bucketNode
    int i;
    for(i=0; i< b->count ;i++){
        if(!strcmp(b->array[i]->walletID->_walletID, bn->walletID->_walletID)){
            // printf("Sender of already exists so add in the beg of the trx list\n");
            insertBEG(b->array[i]->headofList, bn);
            // return SUCCESS;
            break;
        }
    }
    // so if he doesn't add him in the first available position
    // i start searching from b->count because till there they are not available 
    // if count is 3 it means that 0,1,2 are taken so put it in 3
    for(i=b->count; i<b->size; i++){
        if (b->array[i] != NULL) continue;
        b->array[i] = bn;
        b->count ++ ;
        break;
    }
    return SUCCESS;
}

// full balance of a user
int FullBalance ;

int currentAmount(void* data){
    userBitcoin* this = (userBitcoin*)data;
    FullBalance += this->amount ;
    return 1;
}

// calculate the current balance of a wallet
int calculateBalance(LinkedList* userBtc){
    if(userBtc == NULL) return ERROR;
    FullBalance = 0;
    int temp = 0;
    if(doForAll(userBtc, currentAmount) == SUCCESS){
        // executed correctly
        temp = FullBalance;
        // printf("this list's balance is %d \n", temp);
    }
    else{
        return ERROR;
    }
    FullBalance = 0;
    return temp;
}