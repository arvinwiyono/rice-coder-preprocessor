#include "split_join_header.h"

int main(int argc, char* argv[]){
	
	if(argc == 3){
		FILE* input1 = fopen(argv[1], "r");
		FILE* input2 = fopen(argv[2], "r");
		
		if(input1 == NULL && input2 ==NULL){
			printf("ERROR: File 1 and File 2 could not be found in the directory!\n");
			return 0;
		}
		
		if(input1 == NULL){
			printf("ERROR: File 1 could not be found in the directory!\n");
			return 0;
		}
		if(input2 == NULL){
			printf("ERROR: File 2 could not be found in the directory!\n");
			return 0;
		}
		
		//If both files found in the directory
		//Setup all required variables
		char magic_num1[3];
		char magic_num2[3];
		int row1, col1, max_val1, row2, col2, max_val2;
		
		//Get header information from input1
		fscanf(input1, "%s", magic_num1);
		getRidComment(input1);
		fscanf(input1, "%d %d %d", &row1, &col1, &max_val1);
		printf("File 1: %s, %d, %d, %d\n", magic_num1, row1, col1, max_val1);
		
		//Get header information from input2
		fscanf(input2, "%s", magic_num2);
		getRidComment(input2);
		fscanf(input2, "%d %d %d", &row2, &col2, &max_val2);
		printf("File 2: %s, %d, %d, %d\n", magic_num2, row2, col2, max_val2);
		
		//Validate max vals
		if(max_val1 > 65536 || max_val1 < 0){
			printf("ERROR: Max value of pixel in File 1 is greater than 65,536 or less than 0!\n");
			fclose(input1);
			return 0;
		}
		
		if(max_val2 > 65536 || max_val2 < 0){
			printf("ERROR: Max value of pixel in File 2 is greater than 65,536 or less than 0!\n");
			fclose(input2);
			return 0;
		}
		
		//Validate magic numbers
		if(strcmp(magic_num1, "P5") == 0 && strcmp(magic_num2, "P5") == 0){
			FILE* output = fopen("original.pgm", "w");
			
			//Get the required number of bits
			int numbit1 = getNumBits(max_val1);
			int numbit2 = getNumBits(max_val2);
			int totalBits = numbit1 + numbit2;
			//debugging
			printf("total bits: %d\n", totalBits);
			
			//Convert both maximum values to binary strings and join them
			char maxValString1[numbit1];
			char maxValString2[numbit2];
			char joinValString[totalBits];
			
			convertToBinString(maxValString1, numbit1, max_val1);
			convertToBinString(maxValString2, numbit2, max_val2);
			joinBinString(maxValString1, maxValString2, joinValString);
			
			unsigned int currentMax = extractBitsToNum(joinValString, 0, totalBits-1);
			
			
			fprintf(output, "P5\n");
			fprintf(output, "%d\n", row1);
			fprintf(output, "%d\n", col1);
			//write the maximum pixel value of the original picture to the output
			fprintf(output, "%d\n", currentMax);
			
			//Setup required variables
			unsigned char symbol1, symbol2, pixel1, pixel2;
			char binary_str1[numbit1+1];
			char binary_str2[numbit2+1];
			
			
			//Get rid of new line char
			fgetc(input1);
			fgetc(input2);
					
			//Get one byte from each file
			symbol1 = fgetc(input1);
			symbol2 = fgetc(input2);
			
			while(!feof(input1) && !feof(input2)){
				char result_string[17] = "";
								
				convertToBinString(binary_str1, numbit1, symbol1);
				convertToBinString(binary_str2, numbit2, symbol2);
								
				int index;
				for(index = 0; index < (16 - totalBits); index++){
					strcat(result_string, "0");
				}
				strcat(result_string, binary_str1);
				strcat(result_string, binary_str2);
				
				pixel1 = extractBitsToNum(result_string, 0, 7);
				pixel2 = extractBitsToNum(result_string, 8, 15);
				
				fprintf(output, "%c%c", pixel1,pixel2);
				symbol1 = fgetc(input1);
				symbol2 = fgetc(input2);
			}
			fclose(output);
			
		}
		else{
			printf("ERROR: Cannot read magic number 'P5' in one of the files!\n");
		}
		
		fclose(input1);
		fclose(input2);
		
	}
	else{
		printf("COMMAND - join_pgm pathtofile1 pathtofile2\n");
		printf("EXAMPLE: join_pgm filename1.pgm filename2.pgm\n");
	}
	return 0;
}
