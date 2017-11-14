// Author: Connor Baker
// Created: November 14, 2017
// Version: 0.1b


#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define DECIMAL_ACCURACY 8192 // Todo: Set this value at runtime

mpf_t numberToConvert;
mpf_t desiredBase;
int accuracy;
//int DECIMAL_ACCURACY;
char *heap;


void convertBaseToBase(char *heap) {
	char wholePartOfProduct;
	mpf_t currentNumber;
	mpf_init2(currentNumber, DECIMAL_ACCURACY);
	mpf_set(currentNumber, numberToConvert);

	for (int i = 2; i < accuracy + 2; i++) {
		mpf_mul(currentNumber, currentNumber, desiredBase);
		wholePartOfProduct = (char) mpf_get_d(currentNumber);
//		gmp_printf("i = %d \t current num is: %.Ff\n", i, currentNumber);
//		gmp_printf("\t whole part is: %d\n", wholePartOfProduct);
		heap[i] = (char) (wholePartOfProduct + '0');
		mpf_sub_ui(currentNumber, currentNumber, (unsigned long) wholePartOfProduct);
	}
}


int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Example usage: ChangeOfBase numberToConvert desiredBase accuracy\n");
		printf("This only works for numbers between 0 and 1, and bases between 1 and 10\n");
		return 0;
	}

	heap = (char *) calloc((size_t) (accuracy + 3L), sizeof(char));

	// Set values and print them to establish that the program is working as expected
	mpf_init2(numberToConvert, DECIMAL_ACCURACY);
	mpf_set_str(numberToConvert, argv[1], 10);
	gmp_printf("numberToConvert: %.Ff\n", numberToConvert);
	mpf_init2(desiredBase, DECIMAL_ACCURACY);
	mpf_set_str(desiredBase, argv[2], 10);
	gmp_printf("desiredBase: %.Ff\n", desiredBase);
	accuracy = (int) strtol((const char *) argv[3], NULL, 10);
	printf("accuracy: %d\n", accuracy);

//	DECIMAL_ACCURACY = accuracy * 4;

	heap[0] = '0';
	heap[1] = '.';
	heap[accuracy+2] = '\0'; // No more trailing garbage
	convertBaseToBase(heap);

	printf("conversion is %s\n", heap);

	return 0;
}