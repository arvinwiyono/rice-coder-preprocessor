#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUMOFTHRESHOLDDIGIT 20
#define NUMOFSYMBOL 256

typedef struct mappingtable{
	int counter;		//To keep track how many times it is used
	unsigned char symbolArray[NUMOFSYMBOL];
	unsigned char rankArray[NUMOFSYMBOL];
}MappingTable;

//GLOBAL VARIABLES
int threshold;
int number_of_symbols;
MappingTable* tables[NUMOFSYMBOL];
MappingTable* initialTable;


void initializeArray(unsigned char array[]);
void initializeTable(MappingTable* table);
void initializeAllTables();
void printArray(unsigned char array[]);
void printTable(MappingTable* table);
void printAllTables();
void printThresholdError();
void printWrongCommand();
void swap(unsigned char array[], int index1, int index2);
void swapNext(MappingTable* table, unsigned char c);
void swapToFront(MappingTable* table, unsigned char c);

