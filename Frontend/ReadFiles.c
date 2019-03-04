#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include "../HeaderFiles/Input.h"
#include "../HeaderFiles/Structs.h"

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
    userBitcoin* temp = (userBitcoin *)t;
   
    printf("this bitcoin's id is : %d and the user owns %d amount \n",  temp->btc->_bitcoinID, temp->amount);
    return 1;
}

int InputManager(struct walletHT* wHT, struct BitcoinHT* bht, char *file, int btcVal){
    FILE* input = FileRead(file);
    if(input == NULL){
        printf("Couldn't Load Input File\n");
        return ERROR;
    }
    // char word[255];
    char *walletID;
    char *bitcoinID;
    int _id;
    int balance = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    const char s[2] = " ";
    char *token;

    walletID = (char *)malloc(50);
    bitcoinID = (char *)malloc(40);
    // initializing the two hts
    wHT = new(WALLET_NUM);
    bht = newBTC(BITCOINS_NUM);

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
        doForAll(ll, printuserBTC);
        balance = calculateBalance(ll);
        printf("the name is %s \n", walletID);
        wal = newWallet(walletID, ll, balance);
        printf("The insertion of wallet is ok! \n");
        insert(wHT, wal);

        printf("The insertion of wallet in HT is ok! \n");

    }

    free(line);
    fclose(input);
    free(walletID);
    free(bitcoinID);
    return SUCCESS;
}