#ifndef TREE_HEADER
#define TREE_HEADER
#include "Structs.h"

struct btcTree {
    struct btcNode node;
    struct btcTree* lKid;
    struct btcTree* rKid;
};

struct Tree {
    struct btcTree* root;
};

typedef struct Tree Tree;
typedef struct btcTree btcTree;

void createTree(Tree* r);
void destroyTree(Tree* r);
btcTree* newTreeNode(btcNode value);
btcTree* TreeSearch(btcTree* root, btcNode value);
void printTree(Tree t);
void addLeft(btcTree* node, btcNode value);
void addRight(btcTree* node, btcNode value);

#endif