#include "split_join_header.h"

void getRidComment(FILE* input){
	char buffer[MAXCHARCOMMENT+1];
	unsigned char c;
	c = fgetc(input);
	c = fgetc(input);
	if(c == '#'){
		//printf("Comment found!\n");
		fgets(buffer, MAXCHARCOMMENT, input);
	}
	else{
		ungetc(c, input);
	}
	
}

unsigned char getNumBits(int val){
	int num = 1;
	unsigned char counter = 0;
	while(num <= val){
		num = num << 1;
		counter++;
	}
	return counter;
}

void convertToBinString(char* buffer, int numOfBits, unsigned int num){
	int counter = numOfBits-1;
	while(num > 0){
		if(num%2 == 1){
			buffer[counter] = '1';
		}
		else{
			buffer[counter] = '0';
		}
		counter--;
		num = num >> 1;
	}
	int index;
	for(index = 0; index <= counter; index++){
		buffer[index] = '0';
	}
	buffer[numOfBits] = '\0';
}

unsigned int extractBitsToNum(char* buffer, int from, int to){
	unsigned int sum = 0;
	int counter = 0;
	int index;
	for(index = to; index >= from; index--){
		if(buffer[index] == '1'){
			sum+= 1 << counter;
		}
		counter++;
	}
	
	return sum;
}

void joinBinString(char* str1, char* str2, char* result){
	int index;
	int counter = 0;
	for(index = 0; index < strlen(str1); index++){
		result[counter] = str1[index];
		counter++;
	}
	for(index = 0; index < strlen(str2); index++){
		result[counter] = str2[index];
		counter++;
	}
	result[counter] = '\0';
}

void partition(char* string, unsigned char* result, int residueBits){
	unsigned char num1, num2;
	num2 = extractBitsToNum(string, 16-residueBits, 16-1);
	result[1] = num2;
	num1 = extractBitsToNum(string, (16-residueBits-8), (16-residueBits-1));
	result[0] = num1;
	
	
}

/* FOR TESTING
int main(){
	unsigned char result = getNumBits(1024);
	printf("result: %d\n", result);
	
	char buffer[result];
	convertToBinString(buffer, result, 753);
	printf("buffer: %s\n", buffer);
	
	unsigned int num1 = extractBitsToNum(buffer, 0, 6);
	unsigned int num2 = extractBitsToNum(buffer, 7, 10);
	printf("num1: %d, num2: %d\n", num1, num2);
	return 0;
}*/
