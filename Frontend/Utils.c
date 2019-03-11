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

int bitcoinStatus(int btcID, BitcoinHT* bHT){
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
    int unsp = unspent(this->btcTree->root);
    printf("%d %d %d\n", this->_bitcoinID, this->btcTree->noOfTrxUsed, unsp);
    return SUCCESS;
}

void reqTrxFile(char* fileName){
    // read file
    // and put each line in reqtrxs

}


int reqTrx(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* senderID, char* receiverID, int amount, char* date, char* _time, int btcVal){
    // generate a trxid
    char* _trxId;
    _trxId = getNextTrxID();
    // printf("The trx id generated for this trx is %s\n", _trxId);
    if(processTrx(wHT, bht, sender, receiver, _trxId, senderID, receiverID, amount, date, _time, btcVal) == ERROR){
        printf("RequestTransaction faced an error\n");
        return ERROR;
    }
    return SUCCESS;
}