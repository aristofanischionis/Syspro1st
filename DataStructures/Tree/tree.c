#include <stdio.h> 
#include <stdlib.h>
#include "../../HeaderFiles/Tree.h"

// create a new node & set default nodes
void createTree(Tree* r){
    r = malloc(sizeof(Tree));
    r->root = malloc(sizeof(btcTree));
    r->root = NULL;
}

void destroyTree(Tree* r){

}

btcTree* newTreeNode(btcNode value){
	btcTree* n = (btcTree *)malloc(sizeof(btcTree));
    n->node = value;
	n->lKid = NULL;
    n->lKid = NULL;
	return n;
}

btcTree* TreeSearch(btcTree* root, btcNode value){
		if(root == NULL)
			return NULL;
		else if (root->node._trxID == value._trxID)
			return root;
		else if(root->rKid != NULL)
			search(root->rKid, value);	     
		else
			search(root->lKid, value);

}

btcTree* addLeft(btcTree* node, btcNode value){
    if(node->lKid != NULL){
        fprintf(stderr, "left kid already exists \n");
        exit(EXIT_FAILURE);
    }
    node->lKid = newTreeNode(value);
}

btcTree* addRight(btcTree* node, btcNode value){
    if(node->rKid != NULL){
        fprintf(stderr, "right kid already exists \n");
        exit(EXIT_FAILURE);
    }
    node->rKid = newTreeNode(value);
}
