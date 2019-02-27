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

struct SenderHT {
    int size;
    int count;
    // mpla mpla ht things
    LinkedList** myBuckets; // this is a bucket *
};

struct ReceiverHT {
    int size;
    int count;
    // mpla mpla ht things
    LinkedList** myBuckets; // this is a bucket *
};
// bucket is a LL of bucket Nodes, pointer to next bucket should be included in b size



// 1 for rec 1 for send and one for wallet
// hash table --> of all bitcoins , btcID and pointer in the tree
// but user will have a ll of bitcoins and amount he has


typedef struct BitcoinHT BitcoinHT;
typedef struct walletHT walletHT;
typedef struct SenderHT SenderHT;
typedef struct ReceiverHT ReceiverHT;