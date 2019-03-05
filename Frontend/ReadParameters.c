#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../HeaderFiles/Input.h"
#include "../HeaderFiles/Structs.h"

// void errorHandling(char* message){
//     fprintf(stderr, message);
//     exit(EXIT_FAILURE);
// }

void paramChecker(int n, char* argv[], char* toCheck, char** result){
    int i = 1;
    while( i<n ){
        if( strcmp(argv[i], toCheck) == 0 ){
            if( i < n - 1 ){
                if( argv[i+1][0] ==  '-' ){
                    printf("After %s flag a - was read\n", toCheck);
                    // errorHandling("param not given1 \n");
                }
                printf("%s flag value is: %s\n", toCheck, argv[i+1]);
                strcpy(*result, argv[i+1]);
                return ;
            }
            else{
                printf("Param after %s flag was not given\n", toCheck);
                printf("exiting...\n");
                // errorHandling("param not given2 \n");
            }
        }
        i++;
    }
}

int InputReader(int argc, char *argv[]){
    int n = argc;
    // init all the variables to be read as cmd line arguments
    char* bitCoinBalancesFile;
    bitCoinBalancesFile = (char*) malloc(50);
    strcpy(bitCoinBalancesFile, "");
    char* trxFile;
    trxFile = (char*) malloc(50);
    strcpy(trxFile, "");
    char* btc;
    int btcValue;
    btc = (char*) malloc(10);
    strcpy(btc, "");
    char* h1;
    int h1Num;
    h1 = (char*) malloc(10);
    strcpy(h1, "");
    char* h2;
    int h2Num;
    h2 = (char*) malloc(10);
    strcpy(h2, "");
    char* b;
    int bSize;
    b = (char*) malloc(10);
    strcpy(b, "");

    // read all cmd arguments
    paramChecker(n, argv, "-a", &bitCoinBalancesFile);
    paramChecker(n, argv, "-t", &trxFile);
    paramChecker(n, argv, "-v", &btc);
    btcValue = atoi(btc);
    paramChecker(n, argv, "-h1", &h1);
    h1Num = atoi(h1);
    paramChecker(n, argv, "-h2", &h2);
    h2Num = atoi(h2);
    paramChecker(n, argv, "-b", &b);
    bSize = atoi(b);
    // print them to be sure that everything is right
    printf("So the params list is %s, %s, %d, %d, %d, %d \n", bitCoinBalancesFile, trxFile, btcValue, h1Num, h2Num, bSize);

    // now let's make the walletHT and the Bitcoin HT
    walletHT* wHT;
    BitcoinHT* bHT;
    wHT = new(WALLET_NUM);
    bHT = newBTC(BITCOINS_NUM);
    // so now i need to read the 2 files
    if((strcmp(bitCoinBalancesFile, "") != 0) && (strcmp(trxFile, "") != 0)){
        InputManager(wHT, bHT, bitCoinBalancesFile, trxFile, btcValue);
    }
    else printf("Input File Name for bitCoinBalancesFile not given\n");

    printf("a random wid %s and balance %d \n", wHT->nodes[25]->_walletID, wHT->nodes[25]->balance);
    wallet *wal;
    wal = wHT->nodes[25];
    doForAll(wal->btcList, printuserBTC);
    // and write the data to my structs
    return 0;
}