#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

int MAX = 363746; // random big int
int Unique = YES;

char* getNextTrxID(){
    char* res = malloc(15);
    sprintf(res, "%d", MAX + 1);
    MAX++;
    return res;
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

// int checkUniqueness(void *t)
// {
//     if(t == NULL){
//         printf("No trx ids in this list\n");
//         return 1;
//     }
//     char* temp = *(char **)t;
    
//     printf("this trx id is : %s \n", temp);
//     if(!strcmp(temp, toBeChecked)){
//         // then this id is not unique
//         Unique = NO;
//         return 0;
//     }
//     return 1;
// }
struct tm* checkDateTime(char* date, char* _time){
    // struct tm* res;
    struct tm* res;
    // if(date == NULL || _time == NULL){
    //     time_t rawtime;
    //     char buffer[80];

    //     time(&rawtime);
    //     res = localtime(&rawtime);

    //     strftime(buffer,80,"Now it's %y/%m/%d.",res);
    //     puts(buffer);
    // }

    strptime(_time, "%H:%M:%S", res);
    struct tm * parsedTime; 
    int year, month, day;
    if(sscanf(date, "%d-%d-%d", &day, &month, &year) != EOF){ 
        time_t rawTime;
        time(&rawTime);
        parsedTime = localtime(&rawTime);

        parsedTime->tm_year = year;
        parsedTime->tm_mon = month;
        parsedTime->tm_mday = day;
        // -->
        res->tm_year = parsedTime->tm_year;
        res->tm_mon = parsedTime->tm_mon;
        res->tm_mday = parsedTime->tm_mday;
    }
    return res;
}


int processTrx(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* _trxId, char* senderID, char* receiverID, int value, char* date, char* _time){

    if(wHT == NULL || bht == NULL || sender == NULL || receiver == NULL || senderID == NULL || receiverID == NULL){
        printf("processTrx got wrong input \n");
    }
    // firstly check if trx is valid
    //check if sender and rec ids are valid wallets
    wallet* temp1;
    wallet* temp2;
    struct tm* _timeStruct;
    trxObject* this;
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
    
    // secondly check if it's possible
    if(possibleTrx(temp1, value) == ERROR){
        printf("Sender doesn't have enough money to send\n");
        return ERROR;
    }

    // check date and time validity
    _timeStruct = checkDateTime(date, _time);
    printf("--> I have in my struct : %d-%d-%d and time -> %d:%d\n", _timeStruct->tm_mday, _timeStruct->tm_mon, _timeStruct->tm_year, _timeStruct->tm_hour, _timeStruct->tm_min );
    //do it
    printf("Transaction with id %s is going to be executed right now!\n", _trxId);

    // make the trx object

    this = newTrxObj(temp1, temp2, _trxId, value, _timeStruct);
    if(this == NULL){
        printf("trxobj is null \n");
        return ERROR;
    }
    // check if everything is correct with this trx obj
    // struct tm *temptime = this->_time;
    // printf("------> id %s,v %d send %s rec %s \n", this->_trxID, this->value, this->sender->_walletID, this->receiver->_walletID);
    // printf("trxobj time : %d-%d-%d and time -> %d:%d\n", temptime->tm_mday, temptime->tm_mon, temptime->tm_year, temptime->tm_hour, temptime->tm_min );
    // check done successfully
    //take the sender's btc's trees and add kids


    // add it in both linked lists r and s

    //
    //update all the structs
}