#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include "../HeaderFiles/Input.h"
// #include "../HeaderFiles/Hashtables.h"
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

    while ((nread = getline(&line, &len, input)) != -1) {
        LinkedList* ll = NULL;
        wallet* wal = NULL;
        bitcoin* btc = NULL;
        userBitcoin* ubtc = NULL;

        printf("Retrieved line of length %zu:\n", nread);
        newBtcList(ll);
        // get the first token
        token = strtok(line, s);
        // walletID
        strcpy(walletID, token);

        // walk through other tokens
        while( token != NULL ){
            printf( " %s\n", token );
            token = strtok(NULL, s);
            
            // bitcoinID
            strcpy(bitcoinID, token);
            _id = atoi(bitcoinID);
            // make a new bitcoin
            newBitcoin(btc, _id);
            // make a userbitcoin
            newUserBitcoin(ubtc, btcVal, btc);
            // insert this in the ll 
            insertEND(ll, ubtc);

        }
        // make a linked list of all bitcoins and add it to this wallet

        balance = calculateBalance(ll);
        newWallet(wal, walletID, ll, balance);
        insert(wHT, wal);

    }

    free(line);
    fclose(input);
    free(walletID);
    free(bitcoinID);
    return SUCCESS;
}