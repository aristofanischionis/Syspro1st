#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"
#include "../../HeaderFiles/Input.h"

// global for current max trx id
int MAX ;

// will only be called once when the transaction file is read
// it's goal is to find all the number trxids from the ll of all the trxs
// compare them and find the max, add +1 and make it the max, which i will use later on
// in order to get unique trx ids 
void findMax(LinkedList* list){
    MAX = -1;
    listNode* temp = list->head;
    char* this;
    int myNum = 0;
    while (temp != NULL){
        this = (char*) temp->data;
        for(size_t i = 0; this[i] != '\0'; ++i) {
            if(!isdigit(this[i])) {                
                break;
            }
        }
        myNum = atoi(this);
        if(myNum > MAX){
            MAX = myNum;
        }
        temp = temp->next;
    }
    MAX ++;
    return;
}

// returns the next unique trx id in a string format
char* getNextTrxID(){
    char* res = malloc(15);
    sprintf(res, "%d", MAX);
    MAX++;
    return res;
}

// does the sender has the money needed for this trx?
int possibleTrx(wallet* wal, int value){
    return ((wal->balance >= value) ? YES : NO );
}

// reducing the amount from the sender
void reduceUserBitcoin(userBitcoin* this, int v){
    this->amount = this->amount - v;
}

// incrementing the amount in the receiver
void incrementUserBitcoin(userBitcoin* this, int v){
    this->amount = this->amount + v;
}

// add the new bitcoin in the user's wallet
void addNewUserBitcoin(wallet* this, userBitcoin* ubtc, int v){
    userBitcoin* clone = newUserBitcoin(v, ubtc->btc);
    insertEND(this->btcList, clone);
}

// stop having a bitcoin in his list for the sender if the amount is 0
int deleteBitcoinFromUser(wallet* wal, userBitcoin* ubtc){
    LinkedList* list = wal->btcList;
    listNode* temp = list->head;
    listNode* prev;
    userBitcoin* this;
    // If head node itself holds the key to be deleted 
    if(temp != NULL ){
        this = (userBitcoin*) temp->data;
        if(this->btc->_bitcoinID == ubtc->btc->_bitcoinID){
            list->head = temp->next;
            return SUCCESS;
        }
    }

    // search the node to be deleted
    while(temp != NULL && (this->btc->_bitcoinID != ubtc->btc->_bitcoinID)){
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list 
    if (temp == NULL) return ERROR; 
  
    // Unlink the node from linked list 
    prev->next = temp->next; 

    return SUCCESS;
}

// check if the receiver already has this bitcoin in his wallet
userBitcoin* findInReceiver(wallet* receiver, bitcoin* this){
    listNode *node = receiver->btcList->head;

    while (node != NULL){
        userBitcoin* t;
        t = (userBitcoin*) node->data;

        if(t->btc->_bitcoinID == this->_bitcoinID){
            return t;
        }
        node = node->next;
    }
    return NULL;
}

// calculate the money needed from a specific user bitcoin 
int moneyToTakeFromUbtc(userBitcoin* this, int balanceToReach, int moneyGatheredAlready){
    int thisMoney = this->amount;
    int ineed = 0;
    if(thisMoney + moneyGatheredAlready > balanceToReach){
        ineed = balanceToReach - moneyGatheredAlready;
    }
    else{
        ineed = thisMoney;
    }
    return ineed;
}

// one of the most important functions i have
// iterate through all of my user's bitcoins and find which ones are going to participate in this trx
void findBitcoins(wallet* sender, wallet* receiver, int money, trxObject* this, int btcVal){
    int iNeed = 0;
    listNode *node = sender->btcList->head;
    // int money is the full balance to achieve
    int balanceToReach = money;
    int moneyGatheredAlready = 0;
    int finished = NO;

    while (node != NULL && node->data != NULL && finished == NO)
    {
        userBitcoin* thisUbtc;
        thisUbtc = (userBitcoin*) node->data;
        // printf("I am doin this loop again id : %d, amount %d\n", thisUbtc->btc->_bitcoinID, thisUbtc->amount);

        if(thisUbtc->amount == 0){
            // printf("this is an empty btc %d shouldn't be in this list\n", thisUbtc->btc->_bitcoinID);
            return ;
        }

        // first do wallet things
        // check how much money should I take out of this ubtc
        iNeed = moneyToTakeFromUbtc(thisUbtc, balanceToReach, moneyGatheredAlready);
        moneyGatheredAlready += iNeed;
        reduceUserBitcoin(thisUbtc, iNeed);
        userBitcoin* temp;
        temp = findInReceiver(receiver, thisUbtc->btc);
        if(temp == NULL){
            // rec doesn't have this btc in his list
            // before adding to receiver check if it his first btc to be inserted
            // because then i first need to initialize the bitcoin tre of him
            // if yes first initialize the btc list
            if(receiver->balance == 0){
                initializeBitcoinTrees(receiver, btcVal);
            }
            addNewUserBitcoin(receiver, thisUbtc, iNeed);
        }
        else{
            // he has it so just increment amount
            incrementUserBitcoin(temp, iNeed);
        }
        
        if(thisUbtc->amount == 0){
            // delete it from sender
            if(deleteBitcoinFromUser(sender, thisUbtc) == ERROR){
                printf("I got an error while trying to delete the bitcoin from sender's wallet\n");
            }
        }

        if(moneyGatheredAlready == balanceToReach){
            finished = YES;
        }
        // and now the most important thing 
        // go through the tree to find the leafs to participate in the trx
        updateTree(thisUbtc->btc->btcTree->root, sender, receiver, iNeed, this);
        thisUbtc->btc->btcTree->noOfTrxUsed ++;
        node = node->next;
    }
}

// initialize the latest date to be the smallest at first
// i know that tm_year is supposed to have years from 1900 but in my implementation
// i just put in there the value of the year i want and then print it like a %s

struct tm* initLatest(){
    struct tm* latest;
    latest = malloc(sizeof(struct tm));
    memset(latest, 0, sizeof(struct tm));
    latest->tm_year = 1900;
    latest->tm_mon = 1;
    latest->tm_mday = 1;
    latest->tm_hour = 1;
    latest->tm_min = 1;
    latest->tm_sec = 1;
    return latest;
}

// checks if the id i got is unique
int checkUniqueness(LinkedList* AllTrxs, char* _id){
    // return YES if unique, NO if not
    listNode *node = AllTrxs->head;
    while (node != NULL){
        char* temp;
        temp = (char *)(node->data);
        // printf("temp is %s, id is %s \n", temp, _id);
        if(!strcmp(temp, _id)){
            // if true, temp == id id is not unique
            return NO;
        }
        node = node->next;
    }
    // if node == NULL means it's unique
    char* tempId;
    tempId = malloc(15);
    strcpy(tempId, _id);
    insertBEG(AllTrxs, tempId);
    return YES;
}

// takes date and time string arguments transform them in struct tm* 
// check if they are more than latest and return it
struct tm* checkDateTime(char* date, char* _time, struct tm* latest){
    // diff time initialize sec to 0
    struct tm* res = malloc(sizeof(struct tm));
    memset(res, 0, sizeof(struct tm));

    if(date == NULL){
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        // printf ( "1Current local time and date: %s\n", asctime (timeinfo));
        res->tm_year = timeinfo->tm_year + 1900;
        res->tm_mon = timeinfo->tm_mon;
        res->tm_mday = timeinfo->tm_mday;
    }
    else {
        int year, month, day;
        if(sscanf(date, "%d-%d-%d", &day, &month, &year) != EOF){ 
            res->tm_year = year;
            res->tm_mon = month;
            res->tm_mday = day;
        }
    }
    if(_time == NULL){
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        // printf ( "2Current local time and date: %s\n", asctime (timeinfo));
        res->tm_hour = timeinfo->tm_hour;
        res->tm_min = timeinfo->tm_min;
        res->tm_sec = timeinfo->tm_sec;
    }
    else {
        int hours, minutes;
        // printf("---> %s\n", _time);
        if(sscanf(_time, "%d:%d", &hours, &minutes) != EOF){
            // printf("%d : %d what i read\n", hours, minutes);
            res->tm_hour = hours;
            res->tm_min = minutes;
            res->tm_sec = 0;
        }
    }

    double diffT;
    diffT = difftime(mktime(res), mktime(latest));
    // if < 0 first is less
    // change the latest date time
    if(diffT >= 0){
        // res is later than latest
        // accepted and put as latest
        memcpy(latest, res, sizeof(struct tm));
        // latest = res;
        return res;
    }
    else {
        printf("The time in this transaction is before the latest\n");
        printf("Transaction will be ignored\n");
        return NULL;
    }
    
}

// one of the most fundamental functions
// gets all the arguments
// validates using other functions 
// and executes the transaction
int processTrx(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* _trxId, char* senderID, char* receiverID, int value, char* date, char* _time, int btcVal, struct tm* latest){

    if(wHT == NULL || bht == NULL || sender == NULL || receiver == NULL || senderID == NULL || receiverID == NULL){
        printf("processTrx got wrong input \n");
        return ERROR;
    }
    // firstly check if trx is valid
    if(!strcmp(senderID, receiverID)){
        printf("Sender and Receiver cannot be the same person\n");
        return ERROR;
    }
    if(value == 0){
        printf("The amount transfered from one to another cannot be zero\n");
        return ERROR;
    }
    //check if sender and rec ids are valid wallets
    wallet* temp1;
    wallet* temp2;
    struct tm* _timeStruct;
    trxObject* this;

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
    if(possibleTrx(temp1, value) == NO){
        printf("Sender doesn't have enough money to send\n");
        return ERROR;
    }

    // check date and time validity
    _timeStruct = checkDateTime(date, _time, latest);
    if(_timeStruct == NULL){
        // printf("Something went wrong with the date %s and time %s given \n", date, _time);
        return ERROR;
    }
    // printf("--> I have in my struct : %d-%d-%d and time -> %d:%d\n", _timeStruct->tm_mday, _timeStruct->tm_mon, _timeStruct->tm_year, _timeStruct->tm_hour, _timeStruct->tm_min );
    //do it
    printf("Transaction with id %s is going to be executed right now!\n", _trxId);

    // make the trx object
    this = newTrxObj(senderID, receiverID, _trxId, value, _timeStruct);

    if(this == NULL){
        printf("trxobj is null \n");
        return ERROR;
    }
    // exchange bitcoins between two parties
    // change balances of wallets
    temp1->balance -= value;
    temp2->balance += value;

    // find which ones to give to each other
    //take the sender's btc's trees and add kids
    findBitcoins(temp1, temp2, value, this, btcVal);
    // add it in both linked lists r and s
    insertTransaction(wHT, sender, senderID, this);
    insertTransaction(wHT, receiver, receiverID, this);
    // transaction is completed successfully!
    return SUCCESS;
}