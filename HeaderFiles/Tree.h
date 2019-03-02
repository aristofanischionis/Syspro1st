#ifndef TREE_HEADER
#define TREE_HEADER
#include "Structs.h"

struct btcTree {
    struct btcNode* node;
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
struct btcTree* newTreeNode(struct btcNode* value);
struct btcTree* TreeSearch(struct btcTree* root, struct btcNode* value);
void printTree(Tree t);
void addLeft(struct btcTree* node, struct btcNode* value);
void addRight(struct btcTree* node, struct btcNode* value);

#endif