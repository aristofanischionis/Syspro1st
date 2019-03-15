#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/Tree.h"

void deleteNode(btcTree* node);
void printLeafNodes(btcTree* root);
void printGivenLevel(btcTree *root, int level);
int height(btcTree* node);
int moneyAtGivenLevel(btcTree *root, int level);

// create a new tree
Tree* createTree(int btcVal, char* this){
    Tree* r;
    r = malloc(sizeof(Tree));
    r->root = malloc(sizeof(btcTree));
    r->root->node = newBTCNode(this, btcVal, NULL);
    r->root->lKid = NULL;
    r->root->rKid = NULL;
    r->noOfTrxUsed = 0;
    return r;
}

void deleteLeft(btcTree* node){
    if (node == NULL) return;
    // first delete both subtrees

    deleteLeft(node->lKid);
    deleteBTCnode(node->node);
    // free(node);
}

void deleteRight(btcTree* node){
    if (node == NULL) return;
    // first delete both subtrees
    free(node->node->walletID);
    // deleteRight(node->rKid);
    // free(node);
}

void destroyTree(Tree* r){
    if(r == NULL) return;
    if(r->root == NULL) {
        free(r);
        return;
    }
    // deleteNode(r->root);
    deleteBTCnode(r->root->node);
    deleteLeft(r->root);
    // deleteRight(r->root);
    // free(r);
}
// void deleteNode(btcTree* node){
//     if (node == NULL) return;
//     // first delete both subtrees

//     deleteNode(node->lKid);
//     // deleteNode(node->rKid);

//     // // then delete the node
//     printf("Deleting node with walID %s and dols %d\n", node->node->walletID, node->node->dollars);  
//     // if(node->node->thisTrx != NULL){
//     //     free(node->node->thisTrx);
//     //     // free(node);
//     // }
//     deleteBTCnode(node->node);
//     free(node);
// }
// make a new tree node
btcTree* newTreeNode(btcNode* value){
	btcTree* n = (btcTree*) malloc(sizeof(btcTree));
    // n->node = malloc(sizeof(btcNode));
    n->node = value;
	n->lKid = NULL;
    n->rKid = NULL;
	return n;
}
// add left kid
void addLeft(btcTree* node, btcNode* value){
    if(!node) return;
    node->lKid = newTreeNode(value);
}
// add right kid
void addRight(btcTree* node, btcNode* value){
    if(!node) return;
    node->rKid = newTreeNode(value);
}
// printing the tree
void printTree(Tree t){
    printLeafNodes(t.root);
}
// just for testing
// function to print leaf  
// nodes from left to right 
void printLeafNodes(btcTree* root){
    // if node is null, return 
    if (!root) return;  
    // if node is leaf node, print its data
    if (!root->lKid && !root->rKid)
    { 
        printf("This is a leaf! %s, %d \n", root->node->walletID, root->node->dollars);
    } 
  
    // if left child exists, check for leaf  
    // recursively 
    if (root->lKid) printLeafNodes(root->lKid); 
          
    // if right child exists, check for leaf  
    // recursively 
    if (root->rKid) printLeafNodes(root->rKid); 
}  

// one of the most important functions
// tries to gather the balancefromleafs amount of dollars from this bitcoin's leafs
// so goes through the leafs that have the walletid name in them and getting out money

void updateTree(btcTree* root, wallet* sender, wallet* receiver, int balanceFromLeafs, trxObject* this){
    // if node is null, return 
    if (root == NULL) return; 
    int send = 0;
    int rec = 0;
    // if node is leaf node, and name is walid, get as much balance i can in order to reach balanceFromLeafs 
    if((root->lKid == NULL) && (root->rKid == NULL)){
        if (!strcmp(root->node->walletID, sender->_walletID)){
            if(root->node->dollars >= balanceFromLeafs){
                //after I get money from this leaf I am basically done
                rec = balanceFromLeafs; // this is how much sender will give
                send = root->node->dollars - balanceFromLeafs;
                btcNode* theleftKid; // receiver
                btcNode* therightKid; // sender
                theleftKid = newBTCNode(receiver->_walletID, rec, this);
                therightKid = newBTCNode(sender->_walletID, send, this);
                addLeft(root, theleftKid);
                addRight(root, therightKid);
                return;
            }
            else{
                //after I get money from this leaf I am not done yet
                rec = root->node->dollars; // receiver will get all money from sender
                send = 0; // sender will have no money
                balanceFromLeafs -= rec; 
                // balance left to get from other leafs is -= money received from this leaf and recursively  got to the other leafs as well
                btcNode* theleftKid; // receiver
                btcNode* therightKid; // sender
                theleftKid = newBTCNode(receiver->_walletID, rec, this);
                therightKid = newBTCNode(sender->_walletID, send, this);
                addLeft(root, theleftKid);
                addRight(root, therightKid);
            }        
        } 
    }
    
    // if left child exists, check for leaf
    // recursively 
    if (root->lKid != NULL) updateTree(root->lKid, sender, receiver, balanceFromLeafs, this); 
          
    // if right child exists, check for leaf  
    // recursively 
    if (root->rKid != NULL) updateTree(root->rKid, sender, receiver, balanceFromLeafs, this); 
}

// calculate how much of the money is left to the first owner
int unspent(btcTree* root){
    if(!root) return -1;
    int h = height(root);
    int res = moneyAtGivenLevel(root, h);   
    return res;  
}

// important function to reach leafs of the tree
int height(btcTree* node) 
{ 
    if (node == NULL) 
        return 0; 
    else
    { 
        /* compute the height of each subtree */
        int lheight = 0;
        int rheight = 0;
        if(node->lKid != NULL){
            lheight = height(node->lKid); 
        }
        if(node->rKid != NULL){
            rheight = height(node->rKid); 
        }
        /* use the larger one */
        if (lheight > rheight) 
            return(lheight+1); 
        else return(rheight+1); 
    } 
}
// Function to line by line print trxs
void printTRXs(btcTree *root){ 
    int h = height(root);
    int i;
    for (i=1; i<=h; i++){
        printGivenLevel(root, i); 
    }
} 
  
// Print trxs at a given level 
void printGivenLevel(btcTree *root, int level){ 
    if (!root) 
        return; 
    if (level == 1){
        trxObject* this;
        if(root->node){
            this = root->node->thisTrx;
            if(this != NULL){
                if(this->printed == NO){
                    this->printed = YES;
                    printf("%s %s %s %d ", this->_trxID, this->sender, this->receiver, this->value); 
                    // print time formated
                    printf("%02d-%02d-%d %02d:%02d\n",this->_time->tm_mday, this->_time->tm_mon, this->_time->tm_year, this->_time->tm_hour, this->_time->tm_min );
                }
            }
        }
    }
    else if (level > 1) {
        printGivenLevel(root->lKid, level-1);
        printGivenLevel(root->rKid, level-1); 
    } 
}

int moneyAtGivenLevel(btcTree *root, int level){ 
    if (!root) 
        return 0; 
    if (level == 1){
        return root->node->dollars;
    }
    else if (level > 1) { 
        return moneyAtGivenLevel(root->rKid, level-1);
        // i need all right to go
    }
    else return -1;
} 