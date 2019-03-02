#ifndef INPUT_HEADER
#define INPUT_HEADER
// #include "Hashtables.h"
#include "Structs.h"
int InputReader(int argc, char *argv[]);
int InputManager(struct walletHT* wHT, struct BitcoinHT* bht, char *file, int btcVal);

#endif