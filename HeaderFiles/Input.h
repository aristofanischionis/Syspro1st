#ifndef INPUT_HEADER
#define INPUT_HEADER
// #include "Hashtables.h"
#include "Structs.h"
int InputReader(int argc, char *argv[]);
int InputManager(LinkedList* AllTrxs, walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char *file1, char *file2, int btcVal);
int printuserBTC(void *t);
// call it in transactions if it's the first bitcoin a user gets
void initializeBitcoinTrees(wallet* this, int btcval);
FILE* FileRead (char *in);
void ExitProgram();
int traceCoin(int btcID, BitcoinHT* bHT);

#endif