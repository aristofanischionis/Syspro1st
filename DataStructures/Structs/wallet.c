#include <string.h>
#include "../../HeaderFiles/Structs.h"

wallet* newWallet(
    char _walletID[NUMBER],
    LinkedList *btcList,
    int balance) {

    wallet* n = (wallet *)malloc(sizeof(wallet));
    strcpy(n->_walletID, _walletID);
    n->balance = balance;
    n->btcList = btcList;
    
    return n;
}