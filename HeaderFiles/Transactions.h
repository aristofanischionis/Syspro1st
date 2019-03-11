#include "Structs.h"
void findMax(LinkedList* list);
char* getNextTrxID();
int checkUniqueness(LinkedList* AllTrxs, char* _id);
int processTrx(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* _trxId, char* senderID, char* receiverID, int value, char* date, char* _time, int btcVal);
struct tm* checkDateTime(char* date, char* _time);
int reqTrx(walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char* senderID, char* receiverID, int amount, char* date, char* _time, int btcVal);
void initLatest();