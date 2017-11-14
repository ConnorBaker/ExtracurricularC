// Author: Connor Baker
// Created: November 14, 2017
// Version: 0.1a


#include <stdio.h>
#include <stdlib.h>

long double numberToConvert;
long double desiredBase;
long accuracy;


void convertBaseToBase(char *heap) {
	char floorOfConversion;
	long i;
	for (i = 2; i < accuracy + 2; i++) {
		numberToConvert *= desiredBase;
		floorOfConversion = (char) numberToConvert;
		heap[i] = (char) (floorOfConversion + '0');
		numberToConvert -= floorOfConversion;
	}
}


int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Example usage: ChangeOfBase numberToConvert desiredBase accuracy\n");
		printf("This only works for numbers between 0 and 1, and bases between 1 and 10\n");
		return 0;
	}

	// Set values and print them to establish that the program is working as expected
	numberToConvert = strtold((const char *) argv[1], NULL);
	printf("numberToConvert: %Lf\n", numberToConvert);
	desiredBase = strtold((const char *) argv[2], NULL);
	printf("desiredBase: %Le\n", desiredBase);
	accuracy = strtol((const char *) argv[3], NULL, 10);
	printf("numberToConvert: %li\n", accuracy);

	char newRepresentation[accuracy+2];
	newRepresentation[0] = '0';
	newRepresentation[1] = '.';
	convertBaseToBase(newRepresentation);

	printf("conversion is %s\n", newRepresentation);



	return 0;
}