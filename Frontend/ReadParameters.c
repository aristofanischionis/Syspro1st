#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../HeaderFiles/Input.h"
#include "../HeaderFiles/Structs.h"

// very nice and simple function to read the btc balances file and 
// calculate how many bitcoins i will have and how many wallet ids
// lines will be the same num with the wallets
// bitcoins will be the same number with whitespaces
// good practises for hashtables say that the maximum of a hashtable load should be 70%
// if it id >70% resizeup 2xsize
// <30% load resizedown 2/size
// so i decided to have my hashtables 70% loaded, so don't be overloaded, neither allocate more space than needed

void NumberCalculator(char* btcBalances, int *btcNUM, int *WalNum){
    int counter = 1; // number of wallets
    unsigned long spaces = 1; // number of bitcoins in file
    int ch;
    FILE* input = FileRead(btcBalances);
    while ((ch = fgetc(input)) != EOF){
        if(ch == ' ') spaces ++;
        if(ch == '\n') counter ++;
    }
    fclose(input);
    
    // theoretically in a hashtable the load should be at max 70% of its size
    // 10/7 ~= 1.42
    float num = 1.42;
    *btcNUM = (int)(num * spaces);
    *WalNum = (int)(num * counter);
    return;
}

// calculate how many bucketNode i will have in each bucket
int bucketCalculator(int b){
    // we have to have buckets of b bytes
    // and inside we have to put an array of buckets
    int sizeOfBucketNodeArray = 0;
    sizeOfBucketNodeArray = (int)((b - 2 * sizeof(int)) / sizeof(bucketNode*));
    if(sizeOfBucketNodeArray < 1){
        sizeOfBucketNodeArray = 1;
    }
    printf("I calculated the bucketnodes in each bucket will be %d\n", sizeOfBucketNodeArray);
    return sizeOfBucketNodeArray;
}

// parse command line args
void paramChecker(int n, char* argv[], char* toCheck, char** result){
    int i = 1;
    while( i<n ){
        if( strcmp(argv[i], toCheck) == 0 ){
            if( i < n - 1 ){
                if( argv[i+1][0] ==  '-' ){
                    printf("After %s flag a - was read\n", toCheck);
                    // errorHandling("param not given1 \n");
                }
                // printf("%s flag value is: %s\n", toCheck, argv[i+1]);
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

// bassically play the role of main
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
    free(btc);
    paramChecker(n, argv, "-h1", &h1);
    h1Num = atoi(h1);
    free(h1);
    paramChecker(n, argv, "-h2", &h2);
    h2Num = atoi(h2);
    free(h2);
    paramChecker(n, argv, "-b", &b);
    bSize = atoi(b);
    free(b);
    // print them to be sure that everything is right
    if(bSize < 1 || btcValue < 1 || h1Num < 1 || h2Num < 1){
        printf("Wrong parameters are given to the program\n");
        printf("Exiting now...\n");
        return ERROR;
    }
    // printf("So the params list is %s, %s, %d, %d, %d, %d \n", bitCoinBalancesFile, trxFile, btcValue, h1Num, h2Num, bSize);

    // now let's make the walletHT and the Bitcoin HT
    walletHT* wHT;
    BitcoinHT* bHT;
    SRHashT* sender;
    SRHashT* receiver;
    LinkedList* AllTrxs;
    
    // initializing the trxids ll 
    AllTrxs = init(sizeof(char*), freeString);
    int walNum = 0;
    int btcNum = 0;
    if(!strcmp(bitCoinBalancesFile, "")){
        printf("no bitcoin balances file is given \n");
        return -1;
    }
    NumberCalculator(bitCoinBalancesFile, &btcNum, &walNum);

    printf("Optimal btc and walnums are %d and %d\n", btcNum, walNum);

    wHT = new(walNum);
    bHT = newBTC(btcNum);
    int sizeOfBucketNodeArray = 0;
    sizeOfBucketNodeArray = bucketCalculator(bSize);
    //
    sender = initSRHT(h1Num, sizeOfBucketNodeArray);
    receiver = initSRHT(h2Num, sizeOfBucketNodeArray);
    // so now i need to read the 2 files
    if((strcmp(bitCoinBalancesFile, "") != 0) && (strcmp(trxFile, "") != 0)){
        InputManager(AllTrxs, wHT, bHT, sender, receiver, bitCoinBalancesFile, trxFile, btcValue);
    }
    else printf("Input File Name for bitCoinBalancesFile or Transactions File not given\n");

    return 0;
}
