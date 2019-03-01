#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/Tree.h"
#include "../../HeaderFiles/Hashtables.h"
#include <time.h>
#include <string.h>

void newWallet(wallet* wal, char* _walletID, LinkedList* btcList, int balance){
    wal = malloc(sizeof(wallet));
    strcpy(wal->_walletID, _walletID);
    wal->balance = balance;
    wal->btcList = btcList;
}

void destroyBitcoin(void* data){
    bitcoin* b = (bitcoin*) data;
    destroyTree(b->btcTree);
}
void destroyUserBitcoin(void* data){
    userBitcoin* d = (userBitcoin*)data;
    destroyBitcoin(d->btc);
}
// void free_string(void *data)
// {
//   free(*(char **)data);
// }

void newUserBitcoin(userBitcoin* bcoin, int amount, bitcoin* b){
    bcoin = malloc(sizeof(userBitcoin));
    bcoin->amount = amount;
    bcoin->btc = b;
}

void newBitcoin(bitcoin* b, int _bitcoinID){
    b = malloc(sizeof(bitcoin));
    b->_bitcoinID = _bitcoinID;
    createTree(b->btcTree);
}

int newBTCNode(btcNode* b, walletHT* ht, char* walletID, int dollars, int txID){
    b = malloc(sizeof(btcNode));
    wallet* res;

    res = search(ht, walletID);
    if (res == NULL) return ERROR;

    b->dollars = dollars;
    b->walletID = res;
    b->_trxID = txID;
    return SUCCESS;
}

void newBtcList(LinkedList* list){
    init(list, sizeof(userBitcoin), destroyUserBitcoin);
}

int newTrxObj(trxObject* trx, SRHashT* ht1, SRHashT* ht2, char* sendID, char* recID, int id, int val, struct tm t){
    trx = malloc(sizeof(trxObject));
    wallet* res1;
    wallet* res2;

    res1 = searchHT(ht1, sendID);
    if (res1 == NULL) return ERROR;

    res2 = searchHT(ht2, recID);
    if (res2 == NULL) return ERROR;

    trx->_trxID = id;
    trx->value = val;
    trx->time = t;
    trx->sender = res1;
    trx->receiver = res2;

    return SUCCESS;
}

void destroyTRXlist(void* data){

}

void newTRXList(LinkedList* list){
    init(list, sizeof(trxinLL), destroyTRXlist);
}

int newTrxLLNode(trxinLL* node, trxObject* t, char* wal, walletHT* ht, btcTree* tptr){
    wallet* res;
    node = malloc(sizeof(trxinLL));
    res = search(ht, wal);
    if (res == NULL) return ERROR;

    node->trx = t;
    node->treePointer = tptr;
    node->walletinTRX = res;
    return SUCCESS;
}

int newBucketNode(bucketNode* bkt, char* wal, walletHT* ht, LinkedList* trxList){
    bkt = malloc(sizeof(bucketNode));
    wallet* res;
    res = search(ht, wal);
    if (res == NULL) return ERROR;
    bkt->walletID = res;
    bkt->headofList = trxList;
    return SUCCESS;
}

void newBucket(bucket* b, int size){
    b = (bucket*) malloc(sizeof(bucket));
    b->size = size;
    b->count = 0;
    b->array = (bucketNode**)malloc(size* sizeof(bucketNode*));
    int i;
    for(i = 0; i<size ; i++){
        b->array[i] = (bucketNode*)malloc(sizeof(bucketNode));
        b->array[i] = NULL;
    }
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

void newBucketList(LinkedList* list){
    init(list, sizeof(bucket), destroyBucketlist);
}
// ll of buckets