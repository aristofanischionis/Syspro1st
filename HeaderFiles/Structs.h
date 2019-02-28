#ifndef STRUCTS_HEADER
#define STRUCTS_HEADER
#include <time.h>
#include "LinkedLists.h"
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
    int size;
    struct bucketNode* array;
};

typedef struct wallet wallet;
typedef struct bitcoin bitcoin;
typedef struct userBitcoin userBitcoin;
typedef struct btcNode btcNode;
typedef struct trxObject trxObject;
typedef struct trxinLL trxinLL;
typedef struct bucketNode bucketNode;
typedef struct bucket bucket;

#endif
