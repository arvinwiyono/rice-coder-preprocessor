#include "context.h"

void chooseToOutput(FILE* output, unsigned char current, unsigned char before){
	if(tables[before]->counter > threshold){
		fprintf(output, "%c", tables[before]->symbolArray[current]);
	}
	else{
		fprintf(output, "%c", initialTable->symbolArray[current]);
	}
}

//pref = 1 --> swap next algorithm
//pref = 2 --> swap to front algorithm
void runContextReRank(FILE* input, FILE* output, int pref){
	char readFirst = 1;		//This is a boolean variable for reading the first char
	
	unsigned char current;
	unsigned char before;
	
	//Initialize number of symbols for swap to front
	number_of_symbols = 0;
	
	current = fgetc(input);
	while(!feof(input)){
		if(readFirst){
			//Read first character
			fprintf(output, "%c", initialTable->symbolArray[current]);
			
			//Only update initial table
			if(pref == 1){
				swapNext(initialTable, current);
			}
			else{
				swapToFront(initialTable, current);
			}
			readFirst = 0;		//set boolean to false
		}
		else{
			chooseToOutput(output, current, before);
			
			//Update both tables with the specified algorithm
			if(pref == 1){
				swapNext(initialTable, current);
				swapNext(tables[before], current);
			}
			else{
				swapToFront(initialTable, current);
				swapToFront(tables[before], current);
			}
		}
		before = current;
		current = fgetc(input);
	}
}

int main(int argc, char* argv[]){
	if(argc == 3){
		//Declare and define input
		FILE* input = stdin;
		//FILE* input = fopen("test.txt", "r");
		
		if(input == NULL){
			printf("Input file not found in the directory!\n");
			return 0;
		}
		
		if(strcmp(argv[1], "-swap_next") == 0 || strcmp(argv[1], "-swap2front") == 0){
			
			//initialize all tables
			initializeAllTables();
			
			//Get the threshold
			int index;
			char tstring[NUMOFTHRESHOLDDIGIT+1] = "";
			
			for(index = 2; index < strlen(argv[2]); index++){
				tstring[index-2] = argv[2][index];
			}
			strcat(tstring, "\0");
			threshold = atoi(tstring);
			
			//Validate threshold
			if(threshold < 0){
				printThresholdError();
				return 0;
			}
			//Declare and define output
			//FILE* output = fopen("pre-processed.txt", "w");
			FILE* output = stdout;
			
			
			if(strcmp(argv[1], "-swap_next") == 0){
				//Run swap next algorithm
				runContextReRank(input, output, 1);
			}
			else if(strcmp(argv[1], "-swap2front") == 0){
				//Run swap to front algorithm
				runContextReRank(input, output, 2);
			}
			fclose(output);
		}
		else{
			printWrongCommand();
		}
		fclose(input);
	}
	else{
		printWrongCommand();
	}
	return 0;
}
