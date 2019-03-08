#ifndef TREE_HEADER
#define TREE_HEADER

typedef struct Tree Tree;
typedef struct btcTree btcTree;
#include "Structs.h"

struct btcTree {
    struct btcNode* node;
    struct btcTree* lKid;
    struct btcTree* rKid;
};

struct Tree {
    struct btcTree* root;
};

Tree* createTree();
void destroyTree(Tree* r);
btcTree* newTreeNode(btcNode* value);
btcTree* TreeSearch(btcTree* root, char* walId);
void updateTree(btcTree* root, wallet* sender, wallet* receiver, int balanceFromLeafs, trxObject* this);

void printTree(Tree t);
void addLeft(btcTree* node, btcNode* value);
void addRight(btcTree* node, btcNode* value);
void deleteNode(btcTree* node);
void printLeafNodes(btcTree* root);

#endif