#ifndef TREE_HEADER
#define TREE_HEADER

typedef struct Tree Tree;
typedef struct btcTree btcTree;
#include "Structs.h"

struct btcTree {
    struct btcNode* node; // data of the tree node
    struct btcTree* lKid; // receiver of the trx
    struct btcTree* rKid; // sender of the trx
};

struct Tree {
    struct btcTree* root;
    int noOfTrxUsed;
};

Tree* createTree(int btcVal, char* this);
void destroyTree(Tree* r);
btcTree* newTreeNode(btcNode* value);
void updateTree(btcTree* root, wallet* sender, wallet* receiver, int balanceFromLeafs, trxObject* this);

void printTree(Tree t);
void addLeft(btcTree* node, btcNode* value);
void addRight(btcTree* node, btcNode* value);
void deleteNode(btcTree* node);
void printLeafNodes(btcTree* root);
void printTRXs(btcTree* root);
int unspent(btcTree* root);

#endif