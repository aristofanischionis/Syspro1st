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

int reqTrx(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* senderID, char* receiverID, int amount, char* date, char* _time, int btcVal, struct tm* latest){
    // generate a trxid
    char* _trxId;
    _trxId = getNextTrxID();
    // printf("The trx id generated for this trx is %s\n", _trxId);
    if(processTrx(wHT, bht, sender, receiver, _trxId, senderID, receiverID, amount, date, _time, btcVal, latest) == ERROR){
        printf("RequestTransaction faced an error\n");
        return ERROR;
    }
    return SUCCESS;
}

void reqTrxFile(char* fileName){
    // read file
    // and put each line in reqtrxs

}

int reqTrxs(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, int btcVal, struct tm* latest){
    char **command;
    char *buffer;
    char *pos;
    const char s[3] = " \n";
    char *token;
    size_t bufsize = 100;
    int j = 0;
    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL){
        perror("Unable to allocate buffer\n");
        exit(1);
    }
    command = (char **)malloc(6 * sizeof(char*));
    // and read next line if it starts with something else than a / and the last char in input is ;
    // i will continue making trxs
    while(1){
        j = 0;
        // read next line
        command = (char **)malloc(6 * sizeof(char*));
        getline(&buffer,&bufsize,stdin);
        token = strtok(buffer, s);
        // taking all words in command
        while( token != NULL ) {
            command[j++] = token;
            token = strtok(NULL, s);
        }
        // check if first argument is q then quit this command and continue with the rest
        if ((pos=strchr(command[0], '\n')) != NULL) *pos = '\0';
        // say it in readme
        if (!strcmp(command[0], "q")){
            break;
        }
        if(j == 3){
            if ((pos=strchr(command[2], ';')) != NULL) *pos = '\0';
            int amount = atoi(command[2]);
            // take of the ;
            reqTrx(wHT, bht, sender, receiver, command[0], command[1], amount, NULL, NULL, btcVal, latest);
        }
        else if(j == 4){
            if ((pos=strchr(command[3], ';')) != NULL) *pos = '\0';
            int amount = atoi(command[2]);
            reqTrx(wHT, bht, sender, receiver, command[0], command[1], amount, command[3], NULL, btcVal, latest);
        }
        else if(j == 5){
            if ((pos=strchr(command[4], ';')) != NULL) *pos = '\0';
            int amount = atoi(command[2]);
            reqTrx(wHT, bht, sender, receiver, command[0], command[1], amount, command[3], command[5], btcVal, latest);
        }
        else {
            printf("I didn't understand this command--> %s\n", command[0]);
            break;
        }
    }
    return SUCCESS;
}

int betweenDates(char* fromTime, char* fromYear, char* toTime, char* toYear, struct tm* current){
    //returns YES or NO
    if(fromTime == NULL){
        //no time specified
        if(fromYear == NULL){
            // no year specified
            // so we need all trxs 
            // so it is between
            return YES;
        }
        else {
            // check if current is in between years not care about time
            double diffT1, diffT2;
            struct tm* from = malloc(sizeof(struct tm));
            struct tm* to = malloc(sizeof(struct tm));
            int year, month, day;
            if(sscanf(fromYear, "%d-%d-%d", &day, &month, &year) != EOF){ 
                from->tm_year = year;
                from->tm_mon = month;
                from->tm_mday = day;
            }
            else {
                return NO;
            }
            from->tm_hour = 0;
            from->tm_min = 0;
            from->tm_sec = 0;
            // do the same for to
            if(sscanf(toYear, "%d-%d-%d", &day, &month, &year) != EOF){ 
                to->tm_year = year;
                to->tm_mon = month;
                to->tm_mday = day;
            }
            else {
                return NO;
            }
            to->tm_hour = 23;
            to->tm_min = 58;
            to->tm_sec = 58;

            diffT1 = difftime(mktime(from), mktime(current));
            diffT2 = difftime(mktime(to), mktime(current));
            // if < 0 prwto mikrotero
            // change the latest date time
            if(diffT1 < 0 && diffT2 > 0){
                // from is less than current 
                // and to is more then current
                // so it is a good on to be printed
                return YES;
            }
            else {
                return NO;
            }
        }
    }
    else {
        // time is specified
        if(fromYear == NULL){
            // no year specified
            int fromHours, fromMinutes;
            int toHours, toMinutes;
            if(sscanf(fromTime, "%d:%d", &fromHours, &fromMinutes) == EOF){
                return NO;
            }
            if(sscanf(toTime, "%d:%d", &toHours, &toMinutes) == EOF){
                return NO;
            }
            if((current->tm_hour > fromHours) && (current->tm_min > fromMinutes) && (current->tm_hour < toHours) && (current->tm_min < toHours)){
                return YES;
            }
            else return NO;    
        }
        else {
            // year is specified
            double diffT1, diffT2;
            struct tm* from = malloc(sizeof(struct tm));
            struct tm* to = malloc(sizeof(struct tm));
            int year, month, day;
            int hours, minutes;
            if(sscanf(fromYear, "%d-%d-%d", &day, &month, &year) != EOF){ 
                from->tm_year = year;
                from->tm_mon = month;
                from->tm_mday = day;
            }
            else {
                return NO;
            }
            if(sscanf(fromTime, "%d:%d", &hours, &minutes) != EOF){
                from->tm_hour = hours;
                from->tm_min = minutes;
                from->tm_sec = 0;
            }
            else {
                return NO;
            }
            // do the same for to
            if(sscanf(toYear, "%d-%d-%d", &day, &month, &year) != EOF){ 
                to->tm_year = year;
                to->tm_mon = month;
                to->tm_mday = day;
            }
            else {
                return NO;
            }
            if(sscanf(toTime, "%d:%d", &hours, &minutes) != EOF){
                to->tm_hour = hours;
                to->tm_min = minutes;
                to->tm_sec = 0;
            }
            else {
                return NO;
            }
            // now i have my structs ready 
            diffT1 = difftime(mktime(from), mktime(current));
            diffT2 = difftime(mktime(to), mktime(current));
            // if < 0 prwto mikrotero
            // change the latest date time
            if(diffT1 < 0 && diffT2 > 0){
                // from is less than current 
                // and to is more then current
                // so it is a good on to be printed
                return YES;
            }
            else {
                return NO;
            }
        }
    }
}

int checkValidityofDates(char* fromTime, char* fromYear, char* toTime, char* toYear){
    if((fromTime == NULL) && (toTime != NULL)){
        return ERROR;
    }
    if((fromTime != NULL) && (toTime == NULL)){
        return ERROR;
    }
    if((fromYear == NULL) && (toYear != NULL)){
        return ERROR;
    }
    if((fromYear != NULL) && (toYear == NULL)){
        return ERROR;
    }
    return SUCCESS;
}

int findPayments(walletHT* wHT, char* senderID, SRHashT* sender, char* fromTime, char* fromYear, char* toTime, char* toYear){
    wallet* this;
    this = search(wHT, senderID);
    if(this == NULL){
        printf("This sender name: %s doesn't exist in wallet Hashtable\n", senderID);
        return ERROR;
    }
    if(checkValidityofDates(fromTime, fromYear, toTime, toYear) == ERROR){
        printf("The dates given in findPayments are not correct\n");
        return ERROR;
    }

    printf("Sender %s, has paid in total %d money in all of his transactions\n",this->_walletID, this->moneySent);
    // let's find all suitable trxs
    bucketNode* senderBuck;
    senderBuck = searchSRHT(sender, senderID);
    listNode *node = senderBuck->headofList->head ;
    
    while (node != NULL){
        trxObject* t;
        t = (trxObject*) node->data;
        // i get all trx objects from ll and if they are good with the given dates/years i print them
        if(betweenDates(fromTime, fromYear, toTime, toYear, t->_time) == YES){
            printf("%s %s %s %d ",t->_trxID, t->sender, t->receiver, t->value);
            // print time formated
            printf("%02d-%02d-%d %02d:%02d",t->_time->tm_mday, t->_time->tm_mon, t->_time->tm_year, t->_time->tm_hour, t->_time->tm_min );
            printf("\n"); 
        }
        node = node->next;
    }
    return SUCCESS;
}

int findEarnings(walletHT* wHT, char* receiverID, SRHashT* receiver, char* fromTime, char* fromYear, char* toTime, char* toYear){
    wallet* this;
    this = search(wHT, receiverID);
    if(this == NULL){
        printf("This receiver name: %s doesn't exist in wallet Hashtable\n", receiverID);
        return ERROR;
    }
    if(checkValidityofDates(fromTime, fromYear, toTime, toYear) == ERROR){
        printf("The dates given in findPayments are not correct\n");
        return ERROR;
    }

    printf("receiver %s, has received in total %d money in all of his transactions\n",this->_walletID, this->moneyReceived);
    // let's find all suitable trxs
    bucketNode* receiverBuck;
    receiverBuck = searchSRHT(receiver, receiverID);
    listNode *node = receiverBuck->headofList->head ;
    
    while (node != NULL){
        trxObject* t;
        t = (trxObject*) node->data;
        // i get all trx objects from ll and if they are good with the given dates/years i print them
        if(betweenDates(fromTime, fromYear, toTime, toYear, t->_time) == YES){
            printf("%s %s %s %d ",t->_trxID, t->receiver, t->receiver, t->value);
            // print time formated
            printf("%02d-%02d-%d %02d:%02d",t->_time->tm_mday, t->_time->tm_mon, t->_time->tm_year, t->_time->tm_hour, t->_time->tm_min );
            printf("\n"); 
        }
        node = node->next;
    }
    return SUCCESS;
}

