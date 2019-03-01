#ifndef HASHTABLES_HEADER
#define HASHTABLES_HEADER
#include "LinkedLists.h"
#include "Structs.h"


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


typedef struct BitcoinHT BitcoinHT;
typedef struct walletHT walletHT;
typedef struct SRHashT SRHashT;

walletHT* new(const int size);
void delHT(walletHT* ht);
void insert(walletHT* ht, wallet* item);
wallet* search(walletHT* ht, char* _id);
void print(walletHT* ht);
// SRHT hashtables
int initSRHT(SRHashT* ht, int h1, int numOfBucketNodes);
int insertSRHT(SRHashT* ht, bucketNode* bkt, char* _id );
int searchSRHT(SRHashT* ht, char* _id, bucketNode* result);
void deleteSRHT(SRHashT* ht);

#endif