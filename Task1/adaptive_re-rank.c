#include "setup_swap.h"

/*Function to run the alogrithm
* pref = 1 -> Run swapNext
* pref = 2 -> Run swapToFront
*/
void runAdaptiveReRank(FILE* input, FILE* output, int pref){
	unsigned char c;
		
	//Call function to initialize the mapping table
	initializeTable(symbolTable);
	initializeTable(rankTable);
	number_of_symbols = 0;
	
	c = fgetc(input);
	while(!feof(input)){
		if(pref == 1){
			swapNextReRank(output, c);
		}
		else if(pref == 2){
			swapToFrontReRank(output, c);
		}
		c = fgetc(input);
	}
}

int main(int argc, char* argv[]){
	
	FILE* input = stdin;
	FILE* output = stdout;
	//FILE* input = stdin;
	if(input == NULL){
		printf("ERROR: Input file not found in the directory!\n");
		return 0;
	}
	//FILE* output = stdout;
	if(argc==2){
		if(strcmp(argv[1], "-swap_next") == 0){
			runAdaptiveReRank(input, output, 1);
		}
		else if(strcmp(argv[1], "-swap2front") == 0){
			runAdaptiveReRank(input, output, 2);
		}
		else{
			printf("Program usage: adaptive_re-rank [-swap_next | -swap2front]\n");
		}
	}
	else{
		printf("Program usage: adaptive_re-rank [-swap_next | -swap2front]\n");
	}
	
	fclose(input);
	fclose(output);
	
	return 0;

}
