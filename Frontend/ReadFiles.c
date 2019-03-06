#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include "../HeaderFiles/Input.h"
#include "../HeaderFiles/Structs.h"
#include "../HeaderFiles/Transactions.h"


FILE* FileRead (char *in){
    FILE * input;
    input = fopen (in, "r");
    if (input == NULL) {
        fprintf(stderr, "Can't open input file %s\n", in);
        return(NULL);
    }
    return input;
}

// Function to print a btc
int printuserBTC(void *t)
{
    if(t == NULL){
        printf("No user bitcoins in this list\n");
        return 0;
    }
    userBitcoin* temp = (userBitcoin *)t;
   
    printf("this bitcoin's id is : %d and the user owns %d amount \n",  temp->btc->_bitcoinID, temp->amount);
    return 1;
}

// int checkUniqueness(char** allTrxIDs, char* toBeChecked){
//     int i = 0;
//     for(i=0;(allTrxIDs[i] != NULL);i++){
        
//         if(!strcmp(allTrxIDs[i], toBeChecked)){
//             // this id already exists in the list
//             printf("ID is --> %d, %s,         %s \n", i, allTrxIDs[i], toBeChecked);
//             return NO;
//         }
//     }
//     // if i made it till here and this id is unique
//     // push it in i position
//     // strcpy(allTrxIDs[i], toBeChecked);
//     // memcpy(allTrxIDs[i], toBeChecked, 15);
//     allTrxIDs[i] = toBeChecked;
//     printf("id is -->%d  %s,         %s \n", i, allTrxIDs[i], toBeChecked);
//     return YES;
// }

int InputManager(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, int trxiDs, char *file1, char *file2, int btcVal){
    FILE* input = FileRead(file1);
    if(input == NULL){
        printf("Couldn't Load BitCoin file\n");
        return ERROR;
    }
    FILE* input1 = FileRead(file2);
    if(input1 == NULL){
        printf("Couldn't Load transactions file\n");
        return ERROR;
    }
    char *_trxId;
    char *walletID;
    char *bitcoinID;
    char *senderID;
    char *receiverID;
    char *date;
    char *_time;
    int _id = 0;
    int balance = 0;
    char *line = NULL;
    size_t len = 0;
    size_t nread;
    const char s[2] = " ";
    char *token;

    _trxId = (char*) malloc(15);
    walletID = (char *)malloc(50);
    bitcoinID = (char *)malloc(40);
    senderID = (char *)malloc(50);
    receiverID = (char *)malloc(50);
    date = (char *)malloc(50);
    _time = (char *)malloc(50);
    // let's read trx file
    printf("Time to read the Bitcoin Balances File!\n");
    
    while ((nread = getline(&line, &len, input)) != -1) {
        LinkedList* ll;
        wallet* wal;
        
        ll = newBtcList();
        // get the first token
        token = strtok(line, s);
        // walletIDs
        strcpy(walletID, token);
        // walk through other tokens
        while( token != NULL ){
            bitcoin* btc;
            userBitcoin* ubtc;
            printf( " %s\n", token );
            token = strtok(NULL, s);
            if(token == NULL) break;
            // bitcoinID
            strcpy(bitcoinID, token);
            _id = atoi(bitcoinID);
            // make a new bitcoin
            btc = newBitcoin(_id);
            // i have to put it to the ht
            insertBTC(bht, btc);
            // make a userbitcoin
            ubtc = newUserBitcoin(btcVal, btc);
            // insert this in the ll 
            insertEND(ll, ubtc);
        }
        // make a linked list of all bitcoins and add it to this wallet
        
        // bitcoin* res = searchBTC(bht, 432);
        // printf("insertion of btc %d is correctly execd\n", res->_bitcoinID);
        // bitcoin* res1 = searchBTC(bht, 541);
        // printf("insertion of btc %d is correctly execd\n", res1->_bitcoinID);
        // userBitcoin* temp = (userBitcoin*)ll->head->data;
        // printf("list's id of head is %d\n", temp->btc->_bitcoinID);
        // doForAll(ll, printuserBTC);
        balance = calculateBalance(ll);
        wal = newWallet(walletID, ll, balance);
        insert(wHT, wal);
        printf("The insertion of wallet in HT is ok! \n");
    }

    // let's read trx file
    printf("Time to read the Transactions File!\n");

    
    // char** allTrxIDs;
    char allTrxIDs[trxiDs][15];
    int i;
    // allTrxIDs = (char**)malloc(trxiDs * sizeof(char*));
    // int i;
    for(i=0;i<trxiDs;i++){
        // allocate space for each trxid
        // allTrxIDs[i] = (char*)malloc(15);
        // allTrxIDs[i] = NULL;
        memset(allTrxIDs[i], '\0', sizeof(allTrxIDs));
    }

    char word[255];
    int counter = 0;
    int value = 0;
    while (fscanf(input1,"%s",word)==1){
        switch(counter % 6) {
            case 0 :
                strcpy(_trxId, word);
                break;
            case 1 :
                strcpy(senderID, word);
                break;
            case 2 :
                strcpy(receiverID, word);
                break;
            case 3 :
                value = atoi(word);
                break;
            case 4 :
                strcpy(date, word);
                break;
            case 5 :
                strcpy(_time, word);
                break;
        }
        counter++;
        if( counter % 6 == 0) {
            // first check if id is unique
            // check that the trxid is unique
            printf("------> %s \n", _trxId);
            for(i=0;i < trxiDs;i++){
                if(!strcmp(allTrxIDs[i], "")){
                    printf("This id in i is null %d\n", i);
                    break;
                }
                if(!strcmp(allTrxIDs[i], _trxId)){
                    // this id already exists in the list
                    printf("This id is not unique %d \n", i);
                    return ERROR;
                }
            }
            // if i made it till here and this id is unique
            // push it in i position
            strcpy(allTrxIDs[i], _trxId);
            // memcpy(allTrxIDs[i], _trxId, 15);
            printf("id is -->%d\n", i);
            // allTrxIDs[i] = _trxId;
            printf("alltrxid tou i %d is %s\n", i, allTrxIDs[i]);
            printf("Id is unique \n");
            if(processTrx(wHT, bht, sender, receiver, _trxId, senderID, receiverID, value, date, _time) == ERROR){
                printf("Program crashed while reading the TransactionsFile\n");
                printf("Exiting....\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("Done reading both files Successfully!\n");
    free(line);
    fclose(input);
    free(walletID);
    free(bitcoinID);
    free(senderID);
    free(receiverID);
    free(date);
    free(_time);
    // maybe i also need to free btc, ubtc, wal, destroy ll
    return SUCCESS;
}