#include "split_join_header.h"

int main(int argc, char* argv[]){
	
	if(argc == 2){
		FILE* input = fopen(argv[1], "r");
		if(input != NULL){
		
			char magic_num[3];
			int row, col, max_val;
			fscanf(input, "%s", magic_num);
			//Get rid comment in the header
			getRidComment(input);
			
			fscanf(input, "%d %d %d", &row, &col, &max_val);
			printf("%s, %d, %d, %d\n", magic_num, row, col, max_val);
			
			if(max_val > 65536 || max_val < 0){
				printf("ERROR: Max value of pixel is greater than 65,536 or less than 0!\n");
				fclose(input);
				return 0;
			}
			
			if(strcmp(magic_num, "P5") == 0){
				
				//If maximum pixel value is greater than 8 bits
				if(max_val > 255){
					FILE* output1 = fopen("image.top8.pgm", "w");
					FILE* output2 = fopen("image.residue.pgm", "w");
					
					//Get total number of required bits to contain the maximum value
					unsigned char totalBits = getNumBits(max_val);
					//Get the residue bits
					unsigned char residueBits = totalBits - 8;
					
					//Convert max val to a binary string
					char maxValString[totalBits+1];
					convertToBinString(maxValString, totalBits, max_val);
					
					//Get the value of the 8 most significant bits
					unsigned int max_val1 = extractBitsToNum(maxValString, 0, 7);
					unsigned int max_val2 = (1<< residueBits)-1;
					
					printf("Number of bits required: %d\n", totalBits);
					
					fprintf(output1, "P5\n");
					fprintf(output1, "%d\n", row);
					fprintf(output1, "%d\n", col);
					//write max value to output1
					fprintf(output1, "%d\n", max_val1);
					
					fprintf(output2, "P5\n");
					fprintf(output2, "%d\n", row);
					fprintf(output2, "%d\n", col);
					//write max value to output2
					fprintf(output2, "%d\n", max_val2);
					
					//Prepare buffer for binary strings
					char binary_str1[9];
					char binary_str2[9];
					char join_str[17];
					
					//Set up required variables
					unsigned char symbol1, symbol2, pixel1, pixel2;
					
					//Get rid of new line char
					fgetc(input);
					
					//Get two bytes at a time
					symbol1 = fgetc(input);
					symbol2 = fgetc(input);
					while(!feof(input)){
						
						//Convert first and second byte to binary string
						convertToBinString(binary_str1, 8, symbol1);
						convertToBinString(binary_str2, 8, symbol2);
						//Join them together and put it into join_str
						joinBinString(binary_str1, binary_str2, join_str);
						
						
						unsigned char result[2];
						partition(join_str, result, residueBits);
						
						pixel1 = result[0];
						pixel2 = result[1];
						
						//Send to both output channels
						fprintf(output1, "%c", pixel1);
						fprintf(output2, "%c", pixel2);
						
						symbol1 = fgetc(input);
						symbol2 = fgetc(input);
					}
					
					fclose(output1);
					fclose(output2);
					printf("Splitting is successful!\n");
				}
				else{
					printf("Pixel values can be fit into one byte. No need to split!\n");
				}
			}
			else{
				printf("ERROR: Cannot read magic cumber 'P5'\n");
			}
		
			fclose(input);
		}
		else{
			printf("ERROR: File not found in the directory!\n");
		}
	}
	else{
		printf("COMMAND - split_pgm pathtofile\n");
		printf("EXAMPLE: split_pgm filename.pgm\n");
	}
	return 0;
}
