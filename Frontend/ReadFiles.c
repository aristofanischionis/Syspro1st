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
    const char s[3] = " \n";
    char *token;

    _trxId = (char*) malloc(15);
    walletID = (char *)malloc(50);
    bitcoinID = (char *)malloc(40);
    senderID = (char *)malloc(50);
    receiverID = (char *)malloc(50);
    date = (char *)malloc(50);
    _time = (char *)malloc(50);
    
    struct tm* latest;
    latest = initLatest();
    // init latest date and time
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
        // if(balance == 0){
        //     // for user that don't have any bitcoins at first ignore whitespace at the end
        //     walletID[strlen(walletID) - 1] = 0;
        // }
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
        // if(wal->btcList->head != NULL){
        //     userBitcoin* tata = (userBitcoin*) wal->btcList->head->data;
        //     printf("in head this wal btc list has btc with id %d and the btc node has the correct walletid: %s\n", tata->btc->_bitcoinID, tata->btc->btcTree->root->node->walletID->_walletID);
        // }
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
            // check uniqueness in ll
            if(checkUniqueness(AllTrxs, _trxId) == NO){
                printf("The id: %s is not unique\n", _trxId);
                printf("Wrong transaction details, Transaction will be ignored\n");
                continue;
            }
            // id is unique let's process the trx
            if(processTrx(wHT, bht, sender, receiver, _trxId, senderID, receiverID, value, date, _time, btcVal, latest) == ERROR){
                // printf("Program crashed while reading the TransactionsFile\n");
                // printf("Exiting....\n");
                // exit(EXIT_FAILURE);
                printf("Wrong transaction details, Transaction will be ignored\n");
                continue;
            }
            // print the tree to see that it is correctly updated
            
        }
    }

    printf("Done reading both files Successfully!\n");
    findMax(AllTrxs);
    
    // destroy(AllTrxs);
    fclose(input);
    fclose(input1);
    
    // printBTC(bht);
    // bucketNode* bkt1 = NULL;
    // bkt1 = searchSRHT(sender, "richard");
    // if(bkt1 != NULL){
    //     printf("%s found in sender hash table and balance he has is %d\n", bkt1->walletID->_walletID, bkt1->walletID->balance);
    // }
    // let's now start reading input from user
    printf("I am available to process any commands you give me: \n");
    // Managing Input from user here:
    char **command;
    char *buffer;
    char *pos;
    const char kat[2] = " ";
    size_t bufsize = 100;
    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL){
        perror("Unable to allocate buffer\n");
        exit(1);
    }
    int maxWordsInCommand = 10; // change to 6 afterwards
    //---------------->
    while(1){
        int i =0;
        command = (char **)malloc(maxWordsInCommand * sizeof(char*));
        printf("> ");
        if( getline(&buffer,&bufsize,stdin) == -1){
            //exits program
            ExitProgram();
            free(command);
            free(buffer);
            free(line);
            //
            free(walletID);
            free(bitcoinID);
            free(senderID);
            free(receiverID);
            free(date);
            free(_time);
            return 0;
        }
        token = strtok(buffer, kat);
        // taking all words in command
        while( token != NULL ) {
            command[i++] = token;
            token = strtok(NULL, kat);
        }
        if ((pos=strchr(command[i-1], '\n')) != NULL) *pos = '\0';
        // Analyzing the command given by user
        switch(i){
            case 1:
                // one argument given
                // if ((pos=strchr(command[0], '\n')) != NULL) *pos = '\0';
                if(!strcmp(command[0], "exit")){ 
                    //exits program
                    ExitProgram();
                    free(command);
                    free(buffer);
                    free(line);
                    //
                    free(walletID);
                    free(bitcoinID);
                    free(senderID);
                    free(receiverID);
                    free(date);
                    free(_time);
                    return 0;
                }
                else fprintf(stderr, "Unknown Command Starting with:%s \n",command[0]);
                break;
            case 2:
                // two arguments given
                if(!strcmp(command[0], "traceCoin")){
                    ///traceCoin bitCoinID
                    // printf("/traceCoin bitCoinID -> %s,%s\n", command[0], command[1]);
                    int _id = atoi(command[1]);
                    traceCoin(_id, bht);
                }
                else if(!strcmp(command[0], "bitCoinStatus")){
                    ///bitCoinStatus bitCoinID
                    // printf("/bitCoinStatus bitCoinID-> %s,%s\n", command[0], command[1]);
                    int _id = atoi(command[1]);
                    bitcoinStatus(_id, bht);
                }
                else if(!strcmp(command[0], "walletStatus")){
                    ///walletStatus walletid
                    // printf("/walletStatus walletid-> %s,%s\n", command[0], command[1]);
                    walletStatus(command[1], wHT);
                }
                else if(!strcmp(command[0], "requestTransactions")){
                    ///requestTransactions inputfile
                    // printf("/requestTransactions inputfile-> %s,%s\n", command[0], command[1]);
                    reqTrxFile(command[1], wHT, bht, sender, receiver, btcVal, latest);
                }
                else if(!strcmp(command[0], "findPayments")){
                    ///findPayments walletid 
                    // printf("/findPayments walletid -> %s,%s\n", command[0], command[1]);
                    findPayments(wHT, command[1], sender, NULL, NULL, NULL, NULL);
                }
                else if(!strcmp(command[0], "findEarnings")){
                    ///findEarnings walletid
                    // printf("/findEarnings walletid-> %s,%s\n", command[0], command[1]);
                    findEarnings(wHT, command[1], receiver, NULL, NULL, NULL, NULL);
                }
                else fprintf(stderr, "Unknown Command Starting with: %s \n",command[0]);
                break;
            case 3:
                // three arguments given
                fprintf(stderr, "3 Words Unknown Command Starting with: %s \n",command[0]);
                break;
            case 4:
                //four arguments given
                if(!strcmp(command[0], "requestTransaction")){
                    ///requestTransaction sender receiver amount
                    // printf("/requestTransaction -> %s,%s,%s,%s\n", command[0], command[1], command[2],command[3]);
                    int amount = atoi(command[3]);
                    reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, NULL, NULL, btcVal, latest);
                }
                else if(!strcmp(command[0], "requestTransactions")){
                    // /requestTransactions senderWalletID receiverWalletID amount
                    // printf("/requestTransactions-> %s,%s,%s,%s\n", command[0], command[1], command[2],command[3]);
                    if ((pos=strchr(command[3], ';')) != NULL) *pos = '\0';
                    // if we got here i can assume that the last argument has a ; and then there is a \n
                    // i will get all the info of the first transaction given to me
                    // i will execute it
                    int amount = atoi(command[3]);
                    reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, NULL, NULL, btcVal, latest);
                    // process all other trxs
                    reqTrxs(wHT, bht, sender, receiver, btcVal, latest);
                }
                else if(!strcmp(command[0], "findEarnings")){
                    // /findEarnings
                    // printf("findEarnings -> %s,%s,%s,%s\n", command[0], command[1], command[2],command[3]);
                    if(strstr(command[2], ":") != NULL){
                        // then i have time
                        findEarnings(wHT, command[1], receiver, command[2], NULL, command[3], NULL);
                    }
                    else if(strstr(command[2], "-") != NULL){
                        // then i have date
                        findEarnings(wHT, command[1], receiver, NULL, command[2], NULL, command[3]);
                    }
                    else {
                        printf("The arguments given are not date or time %s, %s\n", command[2], command[3]);
                    }
                }
                else if(!strcmp(command[0], "findPayments")){
                    // /findPayments
                    // printf("findPayments-> %s,%s,%s,%s\n", command[0], command[1], command[2],command[3]);
                    // check if i got two times or two dates
                    // if command 2 has : or -
                    if(strstr(command[2], ":") != NULL){
                        // then i have time
                        findPayments(wHT, command[1], sender, command[2], NULL, command[3], NULL);
                    }
                    else if(strstr(command[2], "-") != NULL){
                        // then i have date
                        findPayments(wHT, command[1], sender, NULL, command[2], NULL, command[3]);
                    }
                    else {
                        printf("The arguments given are not date or time %s, %s\n", command[2], command[3]);
                    }
                }
                else fprintf(stderr, "Unknown Command Starting with: %s \n",command[0]);
                break;
            case 5:
                //five arguments given
                if(!strcmp(command[0], "requestTransaction")){
                    ///requestTransaction
                    int amount = atoi(command[3]);
                    // if command 4 has : or -
                    if(strstr(command[4], ":") != NULL){
                        // then i have time
                        reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, NULL, command[4], btcVal, latest);
                    }
                    else if(strstr(command[4], "-") != NULL){
                        // then i have date
                        reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, command[4], NULL, btcVal, latest);
                    }
                    else {
                        printf("The arguments given are not date or time %s, %s\n", command[2], command[3]);
                    } 
                }
                else if(!strcmp(command[0], "requestTransactions")){
                    // /requestTransactions
                    // printf("/requestTransactions -> %s,%s,%s,%s,%s,%s\n", command[0], command[1], command[2],command[3], command[4], command[5]);
                    if ((pos=strchr(command[5], ';')) != NULL) *pos = '\0';
                    // if we got here i can assume that the last argument has a ; and then there is a \n
                    // i will get all the info of the first transaction given to me
                    // i will execute it
                    int amount = atoi(command[3]);
                    // if command 4 has : or -
                    if(strstr(command[4], ":") != NULL){
                        // then i have time
                        reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, NULL, command[4], btcVal, latest);
                    }
                    else if(strstr(command[4], "-") != NULL){
                        // then i have date
                        reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, command[4], NULL, btcVal, latest);
                    }
                    else {
                        printf("The arguments given are not date or time %s, %s\n", command[2], command[3]);
                    }
                    
                    // process all other trxs
                    reqTrxs(wHT, bht, sender, receiver, btcVal, latest);  
                }
                else fprintf(stderr, "5 Words Unknown Command Starting with: %s\n",command[0]);
                break;
            case 6:
                //six arguments given
                if(!strcmp(command[0], "requestTransaction")){
                    ///requestTransaction
                    // printf("/requestTransaction-> %s,%s,%s,%s,%s,%s\n", command[0], command[1], command[2],command[3], command[4], command[5]);
                    int amount = atoi(command[3]);
                    reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, command[4], command[5], btcVal, latest);
                }
                else if(!strcmp(command[0], "requestTransactions")){
                    // /requestTransactions
                    // printf("/requestTransactions -> %s,%s,%s,%s,%s,%s\n", command[0], command[1], command[2],command[3], command[4], command[5]);
                    if ((pos=strchr(command[5], ';')) != NULL) *pos = '\0';
                    // if we got here i can assume that the last argument has a ; and then there is a \n
                    // i will get all the info of the first transaction given to me
                    // i will execute it
                    int amount = atoi(command[3]);
                    reqTrx(wHT, bht, sender, receiver, command[1], command[2], amount, command[4], command[5], btcVal, latest);
                    // process all other trxs
                    reqTrxs(wHT, bht, sender, receiver, btcVal, latest);  
                }
                else if(!strcmp(command[0], "findEarnings")){
                    // /findEarnings
                    // printf("/findEarnings-> %s,%s,%s,%s,%s,%s\n", command[0], command[1], command[2],command[3], command[4], command[5]);
                    findEarnings(wHT, command[1], receiver, command[2], command[3], command[4], command[5]);
                }
                else if(!strcmp(command[0], "findPayments")){
                    // /findPayments
                    // printf("/findPayments-> %s,%s,%s,%s,%s,%s\n", command[0], command[1], command[2],command[3], command[4], command[5]);
                    findPayments(wHT, command[1], sender, command[2], command[3], command[4], command[5]);
                }
                fprintf(stderr, "6 Words Unknown Command Starting with: %s \n",command[0]);
                break;
            default:
                fprintf(stderr,"Unknown Command given with %d words\n", i);
        }
        free(command);
    }

    // free(line);
    // fclose(input);
    // free(walletID);
    // free(bitcoinID);
    // free(senderID);
    // free(receiverID);
    // free(date);
    // free(_time);
    // maybe i also need to free btc, ubtc, wal, destroy ll
    return SUCCESS;
}