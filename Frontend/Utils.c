#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../HeaderFiles/Structs.h"
#include "../HeaderFiles/LinkedLists.h"
#include "../HeaderFiles/Input.h"
#include "../HeaderFiles/Transactions.h"


void ExitProgram(){

}

int traceCoin(int btcID, BitcoinHT* bHT){
    if(bHT == NULL){
        printf("the Bitcoin Hashtable is NULL\n");
        return ERROR;   
    }
    bitcoin* this;
    this = searchBTC(bHT, btcID);
    if(this == NULL){
        printf("the Bitcoin is not in HashTable\n");
        return ERROR;
    }
    printTRXs(this->btcTree->root);
    printf("\n");
    return SUCCESS;
}

int walletStatus(char* _id, walletHT* wHT){
    if(wHT == NULL){
        printf("the wallet Hashtable is NULL\n");
        return ERROR;   
    }
    wallet* this;
    this = search(wHT, _id);
    if(this == NULL){
        printf("the wallet id is not in HashTable\n");
        return ERROR;
    }
    printf("Wallet with ID : %s has %d money remaining in his wallet\n", _id, this->balance);
    return SUCCESS;
}