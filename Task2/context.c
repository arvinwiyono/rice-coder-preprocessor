#include "context.h"


// METHODS INVOLVED IN INITIALIZING ALL TABLES /////////////////////
void initializeArray(unsigned char array[]){
	int i;
	for(i = 0; i < NUMOFSYMBOL; i++){
		array[i] = i;
	}
}

void initializeTable(MappingTable* table){
	table->counter = 0;
	initializeArray(table->symbolArray);
	initializeArray(table->rankArray);
}

void initializeAllTables(){
	initialTable = (MappingTable*) malloc(sizeof(MappingTable));
	initializeTable(initialTable);
	
	int i;
	for(i = 0; i < NUMOFSYMBOL; i++){
		tables[i] = (MappingTable*) malloc(sizeof(MappingTable));
		initializeTable(tables[i]);
	}
}

// DEBUGGING METHODS ///////////////////////////////////////////////
void printArray(unsigned char array[]){
	int i;
	for(i = 0; i < NUMOFSYMBOL; i++){
		if(i != NUMOFSYMBOL-1){
			printf("%d ", array[i]);
		}
		else{
			printf("%d\n", array[i]);
		}
	}
}

void printTable(MappingTable* table){
	printArray(table -> symbolArray);
	printf("\n");
	printArray(table -> rankArray);
}

void printAllTables(){
	printf("INITIAL TABLE\n");
	printTable(initialTable);
	
	printf("\n");
	int i;
	for(i = 0; i < NUMOFSYMBOL; i++){
		printf("\nTABLE %d\n", i);
		printTable(tables[i]);
	}
}

// PRINT ERRORS ////////////////////////////////////////////////
void printThresholdError(){
	printf("ERROR: Threshold is NOT a positive integer !\n");
}

void printWrongCommand(){
	printf("ERROR: Wrong command !\n");
	printf("context_re-rank [-swap_next | -swap2front] -tn\n");
	printf("-tn, where n is a positive number which specifies the threshold.\n");	
}


// SWAPPING ALGORITHMS /////////////////////////////////////////
void swap(unsigned char array[], int index1, int index2){
	unsigned char temp = array[index1];
	array[index1] = array[index2];
	array[index2] = temp;
}

//Swap Next algorithm
void swapNext(MappingTable* table, unsigned char c){
	//Increase table counter
	table->counter++;
	unsigned char symbolC = c;
	
	//Perform swapings
	unsigned char rankC = table->symbolArray[c];
	if(rankC > 0){
		unsigned char symbolSwap = table->rankArray[rankC-1];
		swap(table->symbolArray, symbolC, symbolSwap);
		swap(table->rankArray, rankC, rankC-1);	
	}
}

//Swap to Front algorithm
void swapToFront(MappingTable* table, unsigned char c){
	//Increase table counter
	table->counter++;
	unsigned char symbolC = c;
	
	//Perform swapings
	unsigned char rankC = table->symbolArray[c];
	if(rankC < number_of_symbols){
		//Perform swap next algorithm
		if(rankC > 0){
			unsigned char symbolSwap1 = table->rankArray[rankC-1];
			swap(table->symbolArray, symbolC, symbolSwap1);
			swap(table->rankArray, rankC, rankC-1);
		}
	}
	else{
		unsigned char symbolSwap2 = table->rankArray[number_of_symbols];
		swap(table->symbolArray, symbolC, symbolSwap2);
		swap(table->rankArray, rankC, number_of_symbols);
		number_of_symbols++;
	}
}

