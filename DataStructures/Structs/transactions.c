#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"
#include "../../HeaderFiles/Input.h"

int MAX = 36374; // random big int
int Unique = YES;

char* getNextTrxID(){
    // change it to be more efficient
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
            printf("I found btc id %d in receiver\n", t->btc->_bitcoinID);
            return t;
        }
        node = node->next;
    }
    printf("I couldn't find in receiver the requested id %d\n", this->_bitcoinID);
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
        printf("I am doin this loop again id : %d, amount %d\n", thisUbtc->btc->_bitcoinID, thisUbtc->amount);

        if(thisUbtc->amount == 0){
            printf("this is an empty btc %d shouldn't be in this list\n", thisUbtc->btc->_bitcoinID);
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
            printf("adding bitcoin %d to user %s\n", thisUbtc->btc->_bitcoinID, receiver->_walletID);
            // before adding to receiver check if it his first btc
            // if yes first initialize the btc list
            if(receiver->balance == 0){
                initializeBitcoinTrees(receiver, btcVal);
            }
            addNewUserBitcoin(receiver, thisUbtc, iNeed);
        }
        else{
            // he has it so just increment amount
            printf("he has the bitcoin %d  user %s already\n", thisUbtc->btc->_bitcoinID, receiver->_walletID);
            incrementUserBitcoin(temp, iNeed);
        }
        
        if(thisUbtc->amount == 0){
            // delete it from sender
            printf("I will delete %d from %s user\n", thisUbtc->btc->_bitcoinID, sender->_walletID);
            if(deleteBitcoinFromUser(sender, thisUbtc) == ERROR){
                printf("I got an error while trying to delete the bitcoin from sender's wallet\n");
            }
        }

        if(moneyGatheredAlready == balanceToReach){
            printf("all money is gathered by this ubtc so get it from it's leafs\n");
            finished = YES;
        }
        // if(thisUbtc->btc->btcTree->root == NULL){
        //     printf("I have an unitialized btc tree!!!!!!!!!!!!!!!\n");
        // }
        // else printf("the btc trees are initializedddd!!!!!!!11\n");
        updateTree(thisUbtc->btc->btcTree->root, sender, receiver, iNeed, this);
        
        // this bitcoin is used in one more trx
        thisUbtc->btc->noOfTrxUsed ++;
        node = node->next;

        // printLeafNodes(thisUbtc->btc->btcTree->root);
    }
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

struct tm* checkDateTime(char* date, char* _time){
    // struct tm* res;
    struct tm* res = malloc(sizeof(struct tm));
    // if(date == NULL || _time == NULL){
    //     time_t rawtime;
    //     char buffer[80];

    //     time(&rawtime);
    //     res = localtime(&rawtime);

    //     strftime(buffer,80,"Now it's %y/%m/%d.",res);
    //     puts(buffer);
    // }

    int hours, minutes;
    // printf("---> %s\n", _time);
    if(sscanf(_time, "%d:%d", &hours, &minutes) != EOF){
        // printf("%d : %d what i read\n", hours, minutes);
        res->tm_hour = hours;
        res->tm_min = minutes;
    }
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
    // res->tm_hour = hours;
    // res->tm_min = minutes;
    return res;
}


int processTrx(LinkedList* AllTrxs, walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* _trxId, char* senderID, char* receiverID, int value, char* date, char* _time, int btcVal){

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
    // check uniqueness in ll
    if(checkUniqueness(AllTrxs, _trxId) == NO){
        printf("The id: %s is not unique\n", _trxId);
        return ERROR;
    }

    // secondly check if it's possible
    if(possibleTrx(temp1, value) == NO){
        printf("Sender doesn't have enough money to send\n");
        return ERROR;
    }

    // check date and time validity
    _timeStruct = checkDateTime(date, _time);
    // printf("--> I have in my struct : %d-%d-%d and time -> %d:%d\n", _timeStruct->tm_mday, _timeStruct->tm_mon, _timeStruct->tm_year, _timeStruct->tm_hour, _timeStruct->tm_min );
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
    // exchange bitcoins between two parties
    // chenge balances of wallets
    temp1->balance -= value;
    temp2->balance += value;
    // change 
    // find which ones to give to each other
    //take the sender's btc's trees and add kids
    findBitcoins(temp1, temp2, value, this, btcVal);
    printf("findBitcoins executed successfully!\n");
    

    // add it in both linked lists r and s
    bucketNode* bkt1;
    bucketNode* bkt2;
    bkt1 = searchSRHT(sender, senderID);
    bkt2 = searchSRHT(receiver, receiverID);

    if(bkt1 == NULL){
        // first trx for sender
        // so
        LinkedList* newList = newTRXList();
        insertBEG(newList, this);
        bkt1 = newBucketNode(senderID, wHT, newList);
    }
    else{
        insertBEG(bkt1->headofList, this);
    }
    if(bkt2 == NULL){
        // first trx for rec
        // so
        LinkedList* newList = newTRXList();
        insertBEG(newList, this);
        bkt2 = newBucketNode(receiverID, wHT, newList);
    }
    else{
        insertBEG(bkt2->headofList, this);
    }
    
    insertSRHT(sender, bkt1, senderID);
    insertSRHT(receiver, bkt2, receiverID);

    // update all the structs done (I guess)

    return SUCCESS;
}