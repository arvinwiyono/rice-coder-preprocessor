#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Assume that comment line is no longer than 200 chars
#define MAXCHARCOMMENT 200

void getRidComment(FILE* input);
unsigned char getNumBits(int val);
void convertToBinString(char* buffer, int numOfBits, unsigned int num);
unsigned int extractBitsToNum(char* buffer, int from, int to);
void joinBinString(char* str1, char* str2, char* result);
void partition(char* string, unsigned char* result, int residueBits);
