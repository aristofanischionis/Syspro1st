#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"
#include "../../HeaderFiles/Input.h"

int MAX ;

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

char* getNextTrxID(){
    // check if there is any number in the LL and then take the biggest and then add +1
    // and after reading the file they are not going to give me any trxids
    char* res = malloc(15);
    sprintf(res, "%d", MAX);
    MAX++;
    return res;
}

int possibleTrx(wallet* wal, int value){
    return ((wal->balance >= value) ? YES : NO );
}

void reduceUserBitcoin(userBitcoin* this, int v){
    this->amount = this->amount - v;
}
void incrementUserBitcoin(userBitcoin* this, int v){
    this->amount = this->amount + v;
}

void addNewUserBitcoin(wallet* this, userBitcoin* ubtc, int v){
    userBitcoin* clone = newUserBitcoin(v, ubtc->btc);
    insertBEG(this->btcList, clone);
}


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

userBitcoin* findInReceiver(wallet* receiver, bitcoin* this){
    listNode *node = receiver->btcList->head;

    while (node != NULL){
        userBitcoin* t;
        t = (userBitcoin*) node->data;

        if(t->btc->_bitcoinID == this->_bitcoinID){
            // printf("I found btc id %d in receiver\n", t->btc->_bitcoinID);
            return t;
        }
        node = node->next;
    }
    // printf("I couldn't find in receiver the requested id %d\n", this->_bitcoinID);
    return NULL;
}

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
            // printf("adding bitcoin %d to user %s\n", thisUbtc->btc->_bitcoinID, receiver->_walletID);
            // before adding to receiver check if it his first btc
            // if yes first initialize the btc list
            if(receiver->balance == 0){
                initializeBitcoinTrees(receiver, btcVal);
            }
            addNewUserBitcoin(receiver, thisUbtc, iNeed);
        }
        else{
            // he has it so just increment amount
            // printf("he has the bitcoin %d  user %s already\n", thisUbtc->btc->_bitcoinID, receiver->_walletID);
            incrementUserBitcoin(temp, iNeed);
        }
        
        if(thisUbtc->amount == 0){
            // delete it from sender
            // printf("I will delete %d from %s user\n", thisUbtc->btc->_bitcoinID, sender->_walletID);
            if(deleteBitcoinFromUser(sender, thisUbtc) == ERROR){
                printf("I got an error while trying to delete the bitcoin from sender's wallet\n");
            }
        }

        if(moneyGatheredAlready == balanceToReach){
            // printf("all money is gathered by this ubtc so get it from it's leafs\n");
            finished = YES;
        }
        // if(thisUbtc->btc->btcTree->root == NULL){
        //     printf("I have an unitialized btc tree!!!!!!!!!!!!!!!\n");
        // }
        // else printf("the btc trees are initializedddd!!!!!!!11\n");
        updateTree(thisUbtc->btc->btcTree->root, sender, receiver, iNeed, this);
        thisUbtc->btc->btcTree->noOfTrxUsed = thisUbtc->btc->btcTree->noOfTrxUsed + 1;

        // this bitcoin is used in one more trx
        // thisUbtc->btc->noOfTrxUsed ++;
        // bitcoin* btc;
        // btc = thisUbtc->btc;
        // btc->noOfTrxUsed = btc->noOfTrxUsed + 1;
        // printf("-------------->no of trx used %d \n", btc->noOfTrxUsed);
        node = node->next;

        // printLeafNodes(thisUbtc->btc->btcTree->root);
    }
}

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
    // check that the time i have is later than the latest time
    // if(latest == NULL){
    //     // first transaction so it is the latest
    //     latest = res;
    //     return res;
    // }
    double diffT;
        // printf("--> latest : %d-%d-%d and time -> %d:%d\n", latest->tm_mday, latest->tm_mon, latest->tm_year, latest->tm_hour, latest->tm_min );

        // printf("--> current : %d-%d-%d and time -> %d:%d\n", res->tm_mday, res->tm_mon, res->tm_year, res->tm_hour, res->tm_min );
    diffT = difftime(mktime(res), mktime(latest));
    // if < 0 prwto mikrotero
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
    // check if everything is correct with this trx obj
    // struct tm *temptime = this->_time;
    // printf("------> id %s,v %d send %s rec %s \n", this->_trxID, this->value, this->sender->_walletID, this->receiver->_walletID);
    // printf("trxobj time : %d-%d-%d and time -> %d:%d\n", temptime->tm_mday, temptime->tm_mon, temptime->tm_year, temptime->tm_hour, temptime->tm_min );
    // check done successfully
    // exchange bitcoins between two parties
    // change balances of wallets
    temp1->balance -= value;
    temp2->balance += value;
    // sender just sent an amount
    // temp1->moneySent += value;
    // // receiver just received an amount
    // temp2->moneyReceived += value;
    // find which ones to give to each other
    //take the sender's btc's trees and add kids
    findBitcoins(temp1, temp2, value, this, btcVal);
    // printf("findBitcoins executed successfully!\n");
    // add it in both linked lists r and s
    insertTransaction(wHT, sender, senderID, this);
    insertTransaction(wHT, receiver, receiverID, this);
    return SUCCESS;
}