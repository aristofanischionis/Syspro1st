#include <time.h>
#define NUMBER 50

struct wallet {
    char _walletID[NUMBER]; // this is the UserID
    struct LinkedList* btcList; 
    int balance;
};

struct userBitcoin {
    int amount;
    struct bitcoin* btc; // this is the pointer to the real bitcoin
};

struct bitcoin {
    int _bitcoinID;
    struct tree* btcTree;
};

struct btcNode {
    struct wallet* walletID;
    int dollars;
    struct trxObject* trx;
};

struct btcTree {
    struct btcNode node;
    struct btcTree* lKid;
    struct btcTree* rKid;
};

struct trxObject {
    int _trxID;
    struct wallet* senderID;
    struct wallet* receiverID;
    int value;
    struct tm time; // time
};

struct trxinLL {
    struct trxObject* trx;
    struct wallet* walletID;
    struct btcTree* treePointer;
};

struct bucketNode {
    struct wallet* walletID;
    struct trxinLL* headofList;
};

// bucket is a LL of bucket Nodes, pointer to next bucket should be included in b size
// generic implementation 
struct LinkedList {
    void *data;
    struct LinkedList *next;
};

// generic implementation 
struct HashTable {
    int size;
    void **node; // buckets
};

// 1 for rec 1 for send and one for wallet
// hash table --> of all bitcoins , btcID and pointer in the tree
// but user will have a ll of bitcoins and amount he has

typedef struct wallet wallet;
typedef struct bitcoin bitcoin;
typedef struct userBitcoin userBitcoin;
typedef struct btcNode btcNode;
typedef struct btcTree btcTree;
typedef struct trxObject trxObject;
typedef struct LinkedList LinkedList;
typedef struct HashTable HashTable;
typedef struct trxinLL trxinLL;
typedef struct bucketNode bucketNode;
