#include "setup_swap.h"

/*Function to run the alogrithm
* pref = 1 -> Run swapNext
* pref = 2 -> Run swapToFront
*/
void runAdaptiveDeRank(FILE* input, FILE* output, int pref){
	unsigned char c;
		
	//Call function to initialize the mapping table
	initializeTable(rankTable);
	number_of_symbols = 0;
	
	c = fgetc(input);
	while(!feof(input)){
	
		if(pref == 1){
			swapNextDeRank(output, c);
		}
		else if(pref == 2){
			swapToFrontDeRank(output, c);
		}
		c = fgetc(input);
	}
}

int main(int argc, char* argv[]){
	
	FILE* input = stdin;
	FILE* output = stdout;
	if(input == NULL){
		printf("ERROR: File not found in the directory!\n");
		return 0;
	}
	if(argc==2){
		
		if(strcmp(argv[1], "-swap_next") == 0){
			runAdaptiveDeRank(input, output, 1);
		}
		else if(strcmp(argv[1], "-swap2front") == 0){
			runAdaptiveDeRank(input, output, 2);
		}
		else{
			printf("Program usage: adaptive_de-rank [-swap_next | -swap2front]\n");
		}
	}
	else{
		printf("Program usage: adaptive_de-rank [-swap_next | -swap2front]\n");
	}
	
	fclose(input);
	fclose(output);
	
	return 0;

}
