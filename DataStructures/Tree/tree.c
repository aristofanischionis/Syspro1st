#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../../HeaderFiles/Tree.h"

void deleteNode(btcTree* node);
void printLeafNodes(btcTree* root);
void printGivenLevel(btcTree *root, int level);
int height(btcTree* node);
int moneyAtGivenLevel(btcTree *root, int level);
// create a new node & set default nodes
Tree* createTree(){
    Tree* r;
    r = malloc(sizeof(Tree));
    r->root = malloc(sizeof(btcTree));
    r->root = NULL;
    r->noOfTrxUsed = 0;
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
    if (root->lKid == NULL && root->rKid == NULL)
    { 
        printf("This is a leaf! %s, %d \n", root->node->walletID->_walletID, root->node->dollars);
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
            // printf("This is a final leaf! %s, %d , money needed to finish %d\n", root->node->walletID->_walletID, root->node->dollars, balanceFromLeafs);
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
            // printf("This is a leaf! %s, %d \n", root->node->walletID->_walletID, root->node->dollars);
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
    int h = height(root);
    int res = moneyAtGivenLevel(root, h);   
    return res;  
}

int height(btcTree* node) 
{ 
    if (node==NULL) 
        return 0; 
    else
    { 
        /* compute the height of each subtree */
        int lheight = height(node->lKid); 
        int rheight = height(node->rKid); 
  
        /* use the larger one */
        if (lheight > rheight) 
            return(lheight+1); 
        else return(rheight+1); 
    } 
} 
/* Function to line by line print level order traversal a tree*/
void printTRXs(btcTree *root){ 
    int h = height(root); 
    int i;
    // if(h == 0){
    //     printf("This bitcoin hasn't participated in any transactions yet\n");
    // }
    for (i=1; i<=h; i++) 
    {
        printGivenLevel(root, i); 
        printf("\n"); 
    }
} 
  
/* Print nodes at a given level */
void printGivenLevel(btcTree *root, int level){ 
    if (root == NULL) 
        return; 
    if (level == 1){
        trxObject* this;
        if(root->node != NULL){
            this = root->node->thisTrx;
            if(this != NULL){
                printf("%s %s %s %d ", this->_trxID, this->sender->_walletID, this->receiver->_walletID, this->value); 
                // print time formated
                printf("%02d-%02d-%d %02d:%02d",this->_time->tm_mday, this->_time->tm_mon, this->_time->tm_year, this->_time->tm_hour, this->_time->tm_min );
                // printf("\n"); 
            }
        }
    }
    else if (level > 1) { 
        printGivenLevel(root->lKid, level-1);
        // I only need to print out the transactions from one of the kids each time because they are showing to the same transaction 
        // which is correct cause the same transaction caused the break of the tree
        // printGivenLevel(root->rKid, level-1); 
    } 
}

int moneyAtGivenLevel(btcTree *root, int level){ 
    if (root == NULL) 
        return 0; 
    if (level == 1){
        return root->node->dollars;
    }
    else if (level > 1) { 
        return moneyAtGivenLevel(root->rKid, level-1);
        // i need all right to go
    } 
} 