#ifndef STRUCTS_HEADER
#define STRUCTS_HEADER
#include <time.h>
#include "LinkedLists.h"
#include "Hashtables.h"
#include "Tree.h"
#define ERROR 1
#define SUCCESS 0

struct wallet {
    char* _walletID;
    struct LinkedList* btcList;  // this is a LL of userBitcoin*
    int balance;
};

struct userBitcoin {
    int amount;
    struct bitcoin* btc; // this is the pointer to the real bitcoin
};

struct bitcoin {
    int _bitcoinID;
    struct Tree* btcTree;
};

struct btcNode {
    struct wallet* walletID;
    int dollars;
    int _trxID;
};

struct trxObject {
    int _trxID;
    struct wallet* sender;
    struct wallet* receiver;
    int value;
    struct tm time; // time
};

struct trxinLL {
    struct trxObject* trx;
    struct wallet* walletinTRX;
    struct btcTree* treePointer;
};

struct bucketNode {
    struct wallet* walletID;
    struct LinkedList* headofList; // this is  a LL of trxinLL* objects
};

struct bucket {
    int size; // how many can fit
    int count; // how many are inside already
    struct bucketNode** array;
};

typedef struct wallet wallet;
typedef struct bitcoin bitcoin;
typedef struct userBitcoin userBitcoin;
typedef struct btcNode btcNode;
typedef struct trxObject trxObject;
typedef struct trxinLL trxinLL;
typedef struct bucketNode bucketNode;
typedef struct bucket bucket;

void newWallet(wallet* wal, char* _walletID, LinkedList* btcList, int balance);
void newUserBitcoin(userBitcoin* bcoin, int amount, bitcoin* b);
void newBitcoin(bitcoin* b, int _bitcoinID);
int newBTCNode(btcNode* b, walletHT* ht, char* walletID, int dollars, int txID);
void newBtcList(LinkedList* list);
int newTrxObj(trxObject* trx, SRHashT* ht1, SRHashT* ht2, char* sendID, char* recID, int id, int val, struct tm t);
void newTRXList(LinkedList* list);
int newTrxLLNode(trxinLL* node, trxObject* t, char* wal, walletHT* ht, btcTree* tptr);
int newBucketNode(bucketNode* bkt, char* wal, walletHT* ht, LinkedList* trxList);
void newBucket(bucket* b, int size);
int insertNodeinBucket(bucket* b, bucketNode* bn);
void newBucketList(LinkedList* list);

#endif
