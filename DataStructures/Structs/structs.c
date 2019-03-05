#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/Tree.h"
#include "../../HeaderFiles/LinkedLists.h"

wallet* newWallet(char* _walletID, LinkedList* btcList, int balance){
    wallet* wal;
    wal = malloc(sizeof(wallet));

    wal->_walletID = (char *)malloc(50);
    strcpy(wal->_walletID, _walletID);
    
    wal->balance = balance;
    wal->btcList = btcList;
    return wal;
}

void destroyBitcoin(void* data){
    bitcoin* b = (bitcoin*) data;
    destroyTree(b->btcTree);
}

void destroyUserBitcoin(void* data){
    userBitcoin* d = (userBitcoin*)data;
    destroyBitcoin(d->btc);
}

void freeString(void *data)
{
  free(*(char **)data);
}

userBitcoin* newUserBitcoin(int amount, bitcoin* b){
    userBitcoin* bcoin;
    bcoin = malloc(sizeof(userBitcoin));
    bcoin->amount = amount;
    bcoin->btc = b;
    return bcoin;
}

bitcoin* newBitcoin(int _bitcoinID){
    bitcoin* b;
    b = malloc(sizeof(bitcoin));
    b->_bitcoinID = _bitcoinID;
    createTree(b->btcTree);
    return b;
}

btcNode* newBTCNode(walletHT* ht, char* walletID, int dollars, trxObject* txID){
    btcNode* b;
    b = malloc(sizeof(btcNode));
    wallet* res;

    res = search(ht, walletID);
    if (res == NULL) return NULL;

    b->dollars = dollars;
    b->walletID = res;
    b->thisTrx = txID;
    return b;
}

LinkedList* newBtcList(){
    return init(sizeof(userBitcoin), destroyUserBitcoin);
}

trxObject* newTrxObj(walletHT* wHT, char* sendID, char* recID, char* id, int val, struct tm t){
    trxObject* trx;
    trx = malloc(sizeof(trxObject));
    wallet* res1;
    wallet* res2;

    // verify that these owners are there
    res1 = search(wHT, sendID);
    if (res1 == NULL) return NULL;

    res2 = search(wHT, recID);
    if (res2 == NULL) return NULL;

    trx->_trxID = malloc(15);
    strcpy(trx->_trxID, id);
    trx->value = val;
    trx->time = t;
    trx->sender = res1;
    trx->receiver = res2;

    return trx;
}

void destroyTRXlist(void* data){

}

LinkedList* newTRXList(){
    return init(sizeof(trxinLL*), destroyTRXlist);
}

trxinLL* newTrxLLNode(trxObject* t, char* wal, walletHT* ht, btcTree* tptr){
    trxinLL* node;
    wallet* res;
    node = malloc(sizeof(trxinLL));
    res = search(ht, wal);
    if (res == NULL) return NULL;

    node->trx = t;
    node->walletinTRX = res;
    return node;
}

bucketNode* newBucketNode(char* wal, walletHT* ht, LinkedList* trxList){
    bucketNode* bkt;
    bkt = malloc(sizeof(bucketNode));
    wallet* res;
    res = search(ht, wal);
    if (res == NULL) return NULL;
    bkt->walletID = res;
    bkt->headofList = trxList;
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

int insertNodeinBucket(bucket* b, bucketNode* bn){
    if(b->count == b->size) printf("this bucket is full \n");
    return ERROR;
    // check if the sender or receiver already exists in a bucketNode
    int i;
    for(i=0; i< b->count ;i++){
        if(!strcmp(b->array[i]->walletID->_walletID, bn->walletID->_walletID)){
            printf("Sender of already exists so add in the beg of the trx list\n");
            insertBEG(b->array[i]->headofList, bn);
            return SUCCESS;
        }
    }
    // so if he doesn't add him in the first available position
    // i start searching from b->count because till there they are not available 
    // if count is 3 it means that 0,1,2 are taken so put it in 3
    for(i=b->count; i<b->size; i++){
        if (b->array[i] != NULL) continue;
        b->array[i] = bn;
        b->count ++ ;
    }
    return SUCCESS;
}

void destroyBucketlist(void* data){

}

LinkedList* newBucketList(){
    return init(sizeof(bucket*), destroyBucketlist);
}
// ll of buckets
// full balance of a user
int FullBalance ;

int currentAmount(void* data){
    userBitcoin* this = (userBitcoin*)data;
    FullBalance += this->amount ;
    return 1;
}

int calculateBalance(LinkedList* userBtc){
    if(userBtc == NULL) return ERROR;

    FullBalance = 0;
    int temp = 0;
    if(doForAll(userBtc, currentAmount) == SUCCESS){
        // executed correctly
        temp = FullBalance;
        printf("this list's balance is %d \n", temp);
    }
    else{
        exit(1);
    }
    FullBalance = 0;
    return temp;
}