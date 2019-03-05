#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

int MAX = 0;

int getNextTrxID(){
    return MAX+1;
}

int PossibleTrx(walletHT* ht, char* _id, int value){
    int balance = 0;
    wallet *this;
    this = search(ht, _id);
    if(this == NULL){
        printf("This id is not in the HT of wallets\n");
        return ERROR;
    }
    balance = calculateBalance(this->btcList);
    if( balance >= value ) return SUCCESS;
    return ERROR;
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
    deleteNode(this->btcList, ubtcFinder);
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