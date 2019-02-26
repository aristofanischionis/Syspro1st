#include "../../HeaderFiles/Structs.h"

trxObject* newTrx(
    int _trxID,
    wallet* senderID,
    wallet* receiverID,
    int value,
    struct tm t) {

    trxObject* n = (trxObject *)malloc(sizeof(trxObject));
    n->_trxID = _trxID;
    n->value = value;
    n->time = t;
    n->senderID = senderID;
    n->receiverID = receiverID;
    return n;
}

trxinLL* newTrx( 
    trxObject* trx,
    wallet* walletID,
    btcTree* treePointer) {
    
    trxinLL* n = (trxinLL *) malloc(sizeof(trxinLL));
    n->trx = trx;
    n->walletID = walletID;
    n->treePointer = treePointer;
    return n;
}