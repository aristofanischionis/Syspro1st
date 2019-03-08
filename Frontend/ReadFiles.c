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

//Attention in Transactions, when a user gets a bitcoin for the first time call this one
void initializeBitcoinTrees(wallet* this, int btcval){

    btcNode* t;
    t= newBTCNode(this, btcval, NULL);

    listNode* node = this->btcList->head;

    while(node != NULL){
        userBitcoin* ubtc;
        ubtc = (userBitcoin*) (node->data);
        if(ubtc->btc->btcTree == NULL){
            printf(" ----> null this tree\n");
            
        }
        ubtc->btc->btcTree->root = newTreeNode(t);

        node = node->next;
    }

}

int InputManager(LinkedList* AllTrxs, walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char *file1, char *file2, int btcVal){
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
        // token[strlen(token) - 1] = '\0';
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
        if(balance == 0){
            // for user that don't have any bitcoins at first ignore whitespace at the end
            walletID[strlen(walletID) - 1] = 0;
        }
        wal = newWallet(walletID, ll, balance);
        insert(wHT, wal);
        printf("The insertion of wallet in HT is ok! \n");
        wallet* temp = search(wHT, walletID);
        if(temp == NULL){
            printf("for some reason i got a null wallet for %s\n", walletID);
            return ERROR;
        }
        // add the first node of the tree
        initializeBitcoinTrees(wal, btcVal);
        if(wal->btcList->head != NULL){
            userBitcoin* tata = (userBitcoin*) wal->btcList->head->data;
            printf("in head this wal btc list has btc with id %d and the btc node has the correct walletid: %s\n", tata->btc->_bitcoinID, tata->btc->btcTree->root->node->walletID->_walletID);
        }
    }

    // let's read trx file
    printf("Time to read the Transactions File!\n");

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
            // for(i=0;i < trxiDs;i++){
            //     if(!strcmp(allTrxIDs[i], "")){
            //         break;
            //     }
            //     if(!strcmp(allTrxIDs[i], _trxId)){
            //         // this id already exists in the list
            //         printf("This id is not unique %d \n", i);
            //         return ERROR;
            //     }
            // }
            // // if i made it till here and this id is unique
            // // push it in i position
            // strcpy(allTrxIDs[i], _trxId);
            // id is unique let's process the trx
            if(processTrx(AllTrxs, wHT, bht, sender, receiver, _trxId, senderID, receiverID, value, date, _time) == ERROR){
                // printf("Program crashed while reading the TransactionsFile\n");
                // printf("Exiting....\n");
                // exit(EXIT_FAILURE);
                printf("Wrong transaction details, Transaction will be ignored\n");
                continue;
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