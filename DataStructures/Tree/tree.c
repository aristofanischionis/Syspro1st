#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/Tree.h"

void deleteNode(btcTree* node);
void printLeafNodes(btcTree* root);
// create a new node & set default nodes
Tree* createTree(){
    Tree* r;
    r = malloc(sizeof(Tree));
    r->root = malloc(sizeof(btcTree));
    r->root = NULL;
    return r;
}

void destroyTree(Tree* r){
    if(!r) return;
    if(!r->root) {
        free(r);
        return;
    }
    deleteNode(r->root);
    free(r);
}

void deleteNode(btcTree* node){
    if (!node) return;  

    // first delete both subtrees
    deleteNode(node->lKid);  
    deleteNode(node->rKid);

    // then delete the node
    printf("Deleting node with walID %s and dols %d\n", node->node->walletID->_walletID, node->node->dollars);  
    free(node);
}

btcTree* newTreeNode(btcNode* value){
	btcTree* n = (btcTree*) malloc(sizeof(btcTree));
    // n->node = malloc(sizeof(btcNode));
    n->node = value;
	n->lKid = NULL;
    n->lKid = NULL;
	return n;
}

btcTree* TreeSearch(btcTree* root, char* walId){
		if(!root)
			return NULL;
		else if (!strcmp(root->node->walletID->_walletID, walId))
			return root;
		else if(root->rKid)
			TreeSearch(root->rKid, walId);	     
		else
			TreeSearch(root->lKid, walId);
        
}

void addLeft(btcTree* node, btcNode* value){
    if(!node) return;
    if(node->lKid){
        fprintf(stderr, "left kid already exists \n");
        exit(EXIT_FAILURE);
    }
    node->lKid = newTreeNode(value);
    if(!node->rKid){
        node->rKid = NULL;
    }
}

void addRight(btcTree* node, btcNode* value){
    if(!node) return;
    if(node->rKid){
        fprintf(stderr, "right kid already exists \n");
        exit(EXIT_FAILURE);
    }
    node->rKid = newTreeNode(value);
    if(!node->lKid){
        node->lKid = NULL;
    }
}

void printTree(Tree t){
    printLeafNodes(t.root);
}
// function to print leaf  
// nodes from left to right 
void printLeafNodes(btcTree* root){
    // if node is null, return 
    if (root == NULL) return;  
    // if node is leaf node, print its data
    if (root->lKid = NULL && root->rKid == NULL)
    { 
        printf("This is a leaf! %s, %d \n", root->node->walletID->_walletID, root->node->dollars);
        return; 
    } 
  
    // if left child exists, check for leaf  
    // recursively 
    if (root->lKid != NULL) printLeafNodes(root->lKid); 
          
    // if right child exists, check for leaf  
    // recursively 
    if (root->rKid != NULL) printLeafNodes(root->rKid); 
}  

void updateTree(btcTree* root, wallet* sender, wallet* receiver, int balanceFromLeafs, trxObject* this){
    // if node is null, return 
    if (root == NULL) return; 
    int send = 0;
    int rec = 0;
    // if node is leaf node, and name is walid, get as much balance i can in order to reach balanceFromLeafs  
    if ((root->lKid == NULL) && (root->rKid == NULL) && (!strcmp(root->node->walletID->_walletID, sender->_walletID)))
    { 
        if(root->node->dollars >= balanceFromLeafs){
            //after I get money from this leaf I am basically done
            printf("This is a final leaf! %s, %d , money needed to finish %d\n", root->node->walletID->_walletID, root->node->dollars, balanceFromLeafs);
            rec = balanceFromLeafs; // this is how sender will give
            send = root->node->dollars - balanceFromLeafs;
            btcNode* theleftKid; // receiver
            btcNode* therightKid; // sender
            theleftKid = newBTCNode(receiver, rec, this);
            therightKid = newBTCNode(sender, send, this);
            addLeft(root, theleftKid);
            addRight(root, therightKid);
            return;
        }
        else{
            //after I get money from this leaf I am not done yet
            printf("This is a leaf! %s, %d \n", root->node->walletID->_walletID, root->node->dollars);
            rec = root->node->dollars; // receiver will get all money from sender
            send = 0; // sender will have no money
            balanceFromLeafs -= rec; 
            // balance left to get from other leafs is -= money received from this leaf and recursively  got to the other leafs as well

            btcNode* theleftKid; // receiver
            btcNode* therightKid; // sender
            theleftKid = newBTCNode(receiver, rec, this);
            therightKid = newBTCNode(sender, send, this);
            addLeft(root, theleftKid);
            addRight(root, therightKid);
        }        
    } 
  
    // if left child exists, check for leaf  
    // recursively 
    if (root->lKid != NULL) updateTree(root->lKid, sender, receiver, balanceFromLeafs, this); 
          
    // if right child exists, check for leaf  
    // recursively 
    if (root->rKid != NULL) updateTree(root->rKid, sender, receiver, balanceFromLeafs, this); 
}  

int unspent(btcTree* root){
    if(!root) return -1;

    // go all left till lkid is null
    // then return dollars of it
    return 0; // should return the money that is unspent
}