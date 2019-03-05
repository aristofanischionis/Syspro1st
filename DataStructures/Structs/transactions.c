#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

int MAX = 0;
char* toBeChecked;
int Unique = YES;

int getNextTrxID(){
    return MAX+1;
}

int possibleTrx(wallet* wal, int value){
    return ((wal->balance >= value) ? SUCCESS : ERROR );
}

void reduceUserBitcoin(userBitcoin* this, int v){
    this->amount = this->amount - v;
}
void incrementUserBitcoin(userBitcoin* this, int v){
    this->amount = this->amount + v;
}

void addNewUserBitcoin(wallet* this, userBitcoin* ubtc, int v){
    userBitcoin* clone = newUserBitcoin(v, ubtc->btc);
    insertEND(this->btcList, clone);
}

int ubtcFinder(void *data){
    userBitcoin* temp = (userBitcoin *)data;


}

void deleteBitcoinFromUser(wallet* this, userBitcoin* ubtc){
    SkipNode(this->btcList, ubtcFinder);
}

// this fun will return a LL of bitcoins to be used in this trx
LinkedList* findBitcoins(wallet* sender, int money){
    LinkedList* btcs;
    userBitcoin* t;
    btcs = init(sizeof(userBitcoin*), NULL);
    int available = 0;
    int iNeed = 0;
    listNode *node = sender->btcList->head;

    while (node != NULL)
    {
        t = (userBitcoin*) node->data;
        if((t->amount + available) > money ){
            // which means if i get all of this bitcoin i will give more money
            // i will only use a part of it and return the list
            iNeed = money - available;
            // put this in the btc list
            insertEND(btcs, t);
        }
        // t->amount
        node = node->next;
    }

    // while( available != money &&  )
    // userBitcoin* goodOne;




    // insertEND(btcs, );
}

int checkUniqueness(void *t)
{
    if(t == NULL){
        printf("No trx ids in this list\n");
        return 1;
    }
    char* temp = *(char **)t;
    
    printf("this trx id is : %s \n", temp);
    if(!strcmp(temp, toBeChecked)){
        // then this id is not unique
        Unique = NO;
        return 0;
    }
    return 1;
}

int processTrx(LinkedList* allTrxIDs, walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* _trxId, char* senderID, char* receiverID, int value, char* date, char* _time){

    if(wHT == NULL || bht == NULL || sender == NULL || receiver == NULL || senderID == NULL || receiverID == NULL || date == NULL || _time == NULL){
        printf("processTrx got wrong input \n");
    }
    // firstly check if trx is valid
    //check if sender and rec ids are valid wallets
    wallet* temp1;
    wallet* temp2;
    printf("senderid is %s\n", senderID);
    temp1 = search(wHT, senderID);
    if(temp1 == NULL){
        printf("sender id doesn't have a wallet in the Hashtable\n");
        return ERROR;
    }
    temp2 = search(wHT, receiverID);
    if(temp2 == NULL){
        printf("receiver id doesn't have a wallet in the Hashtable\n");
        return ERROR;
    }
    // check that the trxid is unique
    toBeChecked = malloc(15);
    strcpy(toBeChecked, _trxId);
    doForAll(allTrxIDs, checkUniqueness);
    free(toBeChecked);
    // secondly check if it's possible
    if(possibleTrx(temp1, value) == ERROR){
        printf("Sender doesn't have enough money to send\n");
        return ERROR;
    }
    //do it
    printf("Transaction with id %s is going to be executed right now!\n", _trxId);
    // add this trx id in this list
    insertBEG(allTrxIDs, &_trxId);
    //take the sender's btc's trees and add kids

    // make the trx object

    // add it in both linked lists r and s

    //
    //update all the structs
}