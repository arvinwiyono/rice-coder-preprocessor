#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constant declaration
#define NUMOFSYMBOL 256
#define MAXBUFFER 1000

//Global variables
unsigned char symbolTable[NUMOFSYMBOL];
unsigned char rankTable[NUMOFSYMBOL];
int number_of_symbols;

//Method to initialize and debug tables
void initializeTable(unsigned char table[]);
void printTable(unsigned char table[]);

//General swap method
void swap(unsigned char table[], int index1, int index2);

//Swap algorithms for re-rank
void swapNextReRank(FILE* output, unsigned char c);
void swapToFrontReRank(FILE* output, unsigned char c);

//Swap algorithms for de-rank
void swapNextDeRank(FILE* output, unsigned char c);
void swapToFrontDeRank(FILE* output, unsigned char c);
