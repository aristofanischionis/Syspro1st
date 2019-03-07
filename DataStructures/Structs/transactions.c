#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../HeaderFiles/Structs.h"
#include "../../HeaderFiles/LinkedLists.h"

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

int ubtcFinder(void *data){
    userBitcoin* temp = (userBitcoin *)data;
}

void deleteBitcoinFromUser(wallet* this, userBitcoin* ubtc){
    SkipNode(this->btcList, ubtcFinder);
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

void findBitcoins(wallet* sender, wallet* receiver, int money, trxObject* this){
    int available = 0;
    int iNeed = 0;
    listNode *node = sender->btcList->head;
    int adder = 0;
    int flag = 0;
    while (node != NULL && flag != 1)
    {
        userBitcoin* t;
        t = (userBitcoin*) node->data;

        if(t->amount == 0){
            printf("this is an empty btc %d shouldn't be in this list\n", t->btc->_bitcoinID);
            return ;
        }
        bitcoin* thisOne;
        thisOne = t->btc;

        while(flag != 1){
            // check for nodes that have the sender as walletid
            btcTree* tempo;
            
            tempo = returnLeafNodes(thisOne->btcTree->root, sender->_walletID);
            if(tempo == NULL){
                printf("No more leaf nodes\n");
                break;
                // no more leafs 
            }
            // here check amount available in this leaf
            adder = tempo->node->dollars;
            //

            if((adder + available) > money ){
                // which means if i get all of this bitcoin i will give more money
                // i will only use a part of it and return
                iNeed = money - available;
                adder -= iNeed; 
                // reduce this money needed from userbtc
                reduceUserBitcoin(t, iNeed);
                // add it to rec's list if it's not there
                // check if btc is in rec's list
                userBitcoin* temp;
                temp = findInReceiver(receiver, t->btc);
                if(temp == NULL){
                    // rec doesn't have this btc in his list
                    addNewUserBitcoin(receiver, t, iNeed);
                }
                else{
                    // he has it so just increment amount
                    incrementUserBitcoin(temp, iNeed);
                }
                flag = 1;
            }
            else if(adder + available == money){
                // take all of it 
                iNeed = adder;
                adder=0;
                reduceUserBitcoin(t, iNeed);
                // add it to rec's list
                userBitcoin* temp;
                temp = findInReceiver(receiver, t->btc);
                if(temp == NULL){
                    // rec doesn't have this btc in his list
                    addNewUserBitcoin(receiver, t, iNeed);
                }
                else{
                    // he has it so just increment amount
                    incrementUserBitcoin(temp, iNeed);
                }
                // take it out of the sender's btc list
                deleteBitcoinFromUser(sender, t);
                flag =1;
            }
            else if(adder + available < money){
                // use all of it and got to the next one
                iNeed = adder;
                adder=0;
                reduceUserBitcoin(t, iNeed);
                // add this ineed to money available for next loops
                available += iNeed;
                // add it to rec's list
                userBitcoin* temp;
                temp = findInReceiver(receiver, t->btc);
                if(temp == NULL){
                    // rec doesn't have this btc in his list
                    addNewUserBitcoin(receiver, t, iNeed);
                }
                else{
                    // he has it so just increment amount
                    incrementUserBitcoin(temp, iNeed);
                }
                // delete it from sender
                deleteBitcoinFromUser(sender, t);
                flag = 0;
            }
            else {
                printf("Something weird happened here\n");
            }
            // and now it's a good time to add the kids in t's bitcoin tree
            // a kid
            btcNode* theleftKid; // receiver
            btcNode* therightKid; // sender
            theleftKid = newBTCNode(receiver, iNeed, this);
            therightKid = newBTCNode(sender, adder, this);
            addLeft(tempo, theleftKid);
            addRight(tempo, therightKid);
            // this bitcoin is used in one more trx
            thisOne->noOfTrxUsed ++;
        }

        node = node->next;
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


int processTrx(LinkedList* AllTrxs, walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* _trxId, char* senderID, char* receiverID, int value, char* date, char* _time){

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
    findBitcoins(temp1, temp2, value, this);
    printf("findBitcoins executed successfully!\n");
    
    // add it in both linked lists r and s

    // update all the structs
}