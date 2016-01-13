#include "setup_swap.h"

void initializeTable(unsigned char table[]){
	int i;
	for(i = 0; i < NUMOFSYMBOL; i++){
		table[i] = (unsigned char) i;
	}
}
void printTable(unsigned char table[]){
	int i;
	for(i = 0; i < NUMOFSYMBOL; i++){
		if(i == NUMOFSYMBOL-1){
			printf("%d\n", table[i]);
		}
		else{
			printf("%d ", table[i]);
		}
	}
}

void swap(unsigned char table[], int index1, int index2){
	unsigned char temp = table[index1];
	table[index1] = table[index2];
	table[index2] = temp;
}

//RERANK METHODS
void swapNextReRank(FILE* output, unsigned char c){
	unsigned char symbolC = c;
	unsigned char rankC = symbolTable[c];
	fprintf(output, "%c", rankC);
	if(rankC > 0){
		unsigned char symbolSwap = rankTable[rankC-1];
		swap(symbolTable, symbolC, symbolSwap);
		swap(rankTable, rankC, rankC-1);
	}
}


void swapToFrontReRank(FILE* output, unsigned char c){
	unsigned char symbolC = c;
	unsigned char rankC = symbolTable[c];
	fprintf(output, "%c", rankC);
	if(rankC < number_of_symbols){
		if(rankC > 0){
			unsigned char symbolSwap1 = rankTable[rankC-1];
			swap(symbolTable, symbolC, symbolSwap1);
			swap(rankTable, rankC, rankC-1);
		}
	}
	else{
		unsigned char symbolSwap2 = rankTable[number_of_symbols];
		swap(symbolTable, symbolC, symbolSwap2);
		swap(rankTable, rankC, number_of_symbols);
		number_of_symbols++;
	}
}


//DERANK METHODS
void swapNextDeRank(FILE* output, unsigned char c){
	unsigned char rankC = c;
	fprintf(output, "%c", rankTable[rankC]);
	if(rankC > 0){
		swap(rankTable, rankC, rankC-1);
	}
}


void swapToFrontDeRank(FILE* output, unsigned char c){
	unsigned char rankC = c;
	fprintf(output, "%c", rankTable[rankC]);
	if(rankC < number_of_symbols){
		if(rankC > 0){
			swap(rankTable, rankC, rankC-1);
		}
	}
	else{
		swap(rankTable, rankC, number_of_symbols);
		number_of_symbols++;	
	}
}

