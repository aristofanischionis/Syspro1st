#include <stdio.h> 
#include <stdlib.h>
#include "../../HeaderFiles/Structs.h"

// create a new node & set default nodes
btcTree* newTreeNode(btcNode value){
	btcTree* n = (btcTree *)malloc(sizeof(btcTree));
    n->node = value;
	n->lKid = NULL;
    n->lKid = NULL;
	return n;
}

btcTree* search(btcTree* root, btcNode value){
		if(root == NULL)
			return NULL;
		else if (root->node.trx->_trxID == value.trx->_trxID)
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

btcNode* newNode(wallet* id, int money, trxObject* tra) {
    btcNode* n = (btcNode *)malloc(sizeof(btcNode));
    n->dollars = money;
    n->trx = tra;
    n->walletID = id;
    return n;
}