#include <stdio.h> 
#include <stdlib.h>
#include "../../HeaderFiles/Tree.h"

void deleteNode(btcTree* node);
void printLeafNodes(btcTree* root);
// create a new node & set default nodes
void createTree(Tree* r){
    r = malloc(sizeof(Tree));
    r->root = malloc(sizeof(btcTree));
    r->root = NULL;
}

void destroyTree(Tree* r){
    deleteNode(r->root);
}

void deleteNode(btcTree* node){
    if (node == NULL) return;  

    // first delete both subtrees
    deleteNode(node->lKid);  
    deleteNode(node->rKid);

    // then delete the node
    printf("Deleting node with walID %s and dols %d\n", node->node->walletID->_walletID, node->node->dollars);  
    free(node);
}

btcTree* newTreeNode(btcNode* value){
	btcTree* n = (btcTree *)malloc(sizeof(btcTree));
    n->node = value;
	n->lKid = NULL;
    n->lKid = NULL;
	return n;
}

btcTree* TreeSearch(btcTree* root, btcNode* value){
		if(root == NULL)
			return NULL;
		else if (root->node->thisTrx->_trxID == value->thisTrx->_trxID)
			return root;
		else if(root->rKid != NULL)
			TreeSearch(root->rKid, value);	     
		else
			TreeSearch(root->lKid, value);
        
}

void addLeft(btcTree* node, btcNode* value){
    if(node->lKid != NULL){
        fprintf(stderr, "left kid already exists \n");
        exit(EXIT_FAILURE);
    }
    node->lKid = newTreeNode(value);
}

void addRight(btcTree* node, btcNode* value){
    if(node->rKid != NULL){
        fprintf(stderr, "right kid already exists \n");
        exit(EXIT_FAILURE);
    }
    node->rKid = newTreeNode(value);
}

void printTree(Tree t){
    printLeafNodes(t.root);
}
// function to print leaf  
// nodes from left to right 
void printLeafNodes(btcTree* root){
    // if node is null, return 
    if (!root) return; 
      
    // if node is leaf node, print its data     
    if (!root->lKid && !root->rKid)
    { 
        printf("This is a leaf! %s, %d \n", root->node->walletID->_walletID, root->node->dollars);
        return; 
    } 
  
    // if left child exists, check for leaf  
    // recursively 
    if (root->lKid) printLeafNodes(root->lKid); 
          
    // if right child exists, check for leaf  
    // recursively 
    if (root->rKid) printLeafNodes(root->rKid); 
}  