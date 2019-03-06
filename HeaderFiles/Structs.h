#ifndef STRUCTS_HEADER
#define STRUCTS_HEADER
#include <time.h>
#include "LinkedLists.h"
#define ERROR -1
#define SUCCESS 0
#define WALLET_NUM 100
#define BITCOINS_NUM 100
#define YES 2
#define NO 3


typedef struct wallet wallet;
typedef struct bitcoin bitcoin;
typedef struct userBitcoin userBitcoin;
typedef struct btcNode btcNode;
typedef struct trxObject trxObject;
typedef struct bucketNode bucketNode;
typedef struct bucket bucket;
typedef struct BitcoinHT BitcoinHT;
typedef struct walletHT walletHT;
typedef struct SRHashT SRHashT;

#include "Tree.h"
struct wallet {
    char* _walletID;
    struct LinkedList* btcList;  // this is a LL of userBitcoin*
    int balance; // i will use this
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
    struct trxObject* thisTrx;
};

struct trxObject {
    char* _trxID;
    struct wallet* sender;
    struct wallet* receiver;
    int value;
    struct tm* _time; // time
};

// struct trxinLL {
//     struct trxObject* trx;
//     struct wallet* walletinTRX;
// };

struct bucketNode {
    struct wallet* walletID;
    struct LinkedList* headofList; // this is  a LL of trxobj* objects
};

struct bucket {
    int size; // how many can fit
    int count; // how many are inside already
    struct bucketNode** array;
};


struct walletHT {
    int size;
    int count;
    int baseSize;
    struct wallet** nodes;
};

struct BitcoinHT {
    int size;
    int count;
    int baseSize;
    struct bitcoin** nodes;
};

struct SRHashT {
    int size;
    int bucketNodesNum; // how many bucketNodes in each bucket
    LinkedList** myBuckets; // this is an array of size size and its one is a pointer to a LL of buckets
};

// bucket is a LL of bucket Nodes, pointer to next bucket should be included in b size



// 1 for rec 1 for send and one for wallet
// hash table --> of all bitcoins , btcID and pointer in the tree
// but user will have a ll of bitcoins and amount he has

wallet* newWallet(char* _walletID, LinkedList* btcList, int balance);
userBitcoin* newUserBitcoin(int amount, bitcoin* b);
void destroyUserBitcoin(void* data);
bitcoin* newBitcoin(int _bitcoinID);
btcNode* newBTCNode(walletHT* ht, char* walletID, int dollars, trxObject* txID);
LinkedList* newBtcList();
trxObject* newTrxObj(wallet* sendID, wallet* recID, char* id, int val, struct tm* t);
LinkedList* newTRXList();
// trxinLL* newTrxLLNode(trxObject* t, char* wal, walletHT* ht, btcTree* tptr);
bucketNode* newBucketNode(char* wal, walletHT* ht, LinkedList* trxList);
bucket* newBucket(int size);
int insertNodeinBucket(bucket* b, bucketNode* bn);
LinkedList* newBucketList();
int currentAmount(void* data);
int calculateBalance(LinkedList* userBtc);
void freeString(void *data);
//
// wallet HT
walletHT* new(const int size);
void delHT(walletHT* ht);
void insert(walletHT* ht, wallet* item);
wallet* search(walletHT* ht, char* _id);
void print(walletHT* ht);
// Bitcoin HT
BitcoinHT* newBTC(const int size);
void delHTBTC(BitcoinHT* ht);
void insertBTC(BitcoinHT* ht, bitcoin* item);
bitcoin* searchBTC(BitcoinHT* ht, int _id);
void printBTC(BitcoinHT* ht);
// SRHT hashtables
SRHashT* initSRHT(int h1, int numOfBucketNodes);
int insertSRHT(SRHashT* ht,  bucketNode* bkt, char* _id );
int searchSRHT(SRHashT* ht, char* _id,  bucketNode* result);
void deleteSRHT(SRHashT* ht);

#endif
