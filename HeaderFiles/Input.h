#ifndef INPUT_HEADER
#define INPUT_HEADER
// #include "Hashtables.h"
#include "Structs.h"
int InputReader(int argc, char *argv[]);
int InputManager(LinkedList* AllTrxs, walletHT* wHT, BitcoinHT* bht, SRHashT* sender, SRHashT* receiver, char *file1, char *file2, int btcVal);
int printuserBTC(void *t);
FILE* FileRead (char *in);

#endif