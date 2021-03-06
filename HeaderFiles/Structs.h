#ifndef STRUCTS_HEADER
#define STRUCTS_HEADER
#include <time.h>
#include "LinkedLists.h"
#define ERROR -1
#define SUCCESS 0
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
    char* walletID;
    int dollars;
    struct trxObject* thisTrx;
};

struct trxObject {
    char* _trxID;
    char* sender;
    char* receiver;
    int value;
    int printed; // yes or no
    struct tm* _time; // time
};

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
    LinkedList** myBuckets; // this is an array of size size and each one is a pointer to a LL of buckets
};

// ----------------------- FUNCTIONS ------------------------------- //
wallet* newWallet(char* _walletID, LinkedList* btcList, int balance);
userBitcoin* newUserBitcoin(int amount, bitcoin* b);
bitcoin* newBitcoin(int _bitcoinID, int btcVal, char* this);
btcNode* newBTCNode(char* walletID, int dollars, trxObject* txID);
trxObject* newTrxObj(char* sendID, char* recID, char* id, int val, struct tm* t);
bucketNode* newBucketNode(char* wal, walletHT* ht);
bucket* newBucket(int size);
//-------------------------------->
void destroyUserBitcoin(void* data);
void deleteBTCnode(btcNode* node);
void freeString(void *data);
//-------------------------------->
LinkedList* newBtcList();
LinkedList* newTRXList();
LinkedList* newBucketList();
//-------------------------------->
int insertNodeinBucket(bucket* b, bucketNode* bn);
int currentAmount(void* data);
int calculateBalance(LinkedList* userBtc);
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
bucketNode* searchSRHT(SRHashT* ht, char* _id);
void deleteSRHT(SRHashT* ht);
void insertTransaction(walletHT* wHT, SRHashT* ht, char* _id, trxObject* trx);

#endif
