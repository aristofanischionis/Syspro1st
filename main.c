#include <stdio.h>
#include <stdlib.h> 
#include "./HeaderFiles/Input.h"

int main(int argc, char *argv[]){
    system("clear");
    printf("Bitcoin Syspro 1st Assignment Spring 2019 \n");
    if (InputReader(argc, argv)) return 1;
    else return 0;
}