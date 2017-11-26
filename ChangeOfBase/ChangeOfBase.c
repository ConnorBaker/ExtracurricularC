// Author: Connor Baker
// Created: November 14, 2017
// Version: 0.1d



// Import headers
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>



// We should be able to derive this value from the accuracy of the base inputted
#define DECIMAL_ACCURACY 128



// Define variable we use throughout
mpf_t numberToConvert;
mpf_t desiredBase;
int accuracy;



// Create a dictionary to pull from for any alphabet we need, up to base 36
const char dictionary[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                           'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                           'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                           'U', 'V', 'W', 'X', 'Y', 'Z'};



void convertFractionalPartBaseToBase(char *heap);

void convertWholePartBaseToBase(char *heap);

int main(int argc, char *argv[]);



// Technically the greedy algorithm in reverse, I think?
// Def: Beta Map
void convertFractionalPartBaseToBase(char *heap) {
	char wholePartOfProduct = 0;
	mpf_t currentNumber = {}; // Stop the compiler complaining about initialization
	mpf_init2(currentNumber, DECIMAL_ACCURACY);

	// Make sure we remove the whole part of the number before proceeding
	mpf_sub_ui(currentNumber, numberToConvert, mpf_get_ui(numberToConvert));

	for (int i = 0; i < accuracy; i++) {
		mpf_mul(currentNumber, currentNumber, desiredBase);
		wholePartOfProduct = (char) mpf_get_d(currentNumber);
		heap[i] = dictionary[wholePartOfProduct];
		mpf_sub_ui(currentNumber, currentNumber,
		           (unsigned long) wholePartOfProduct);
	}
}



//void convertWholePartBaseToBase(char *heap) {
//	int floorOfBase = (int) mpf_get_d(desiredBase);
//	mpf_t currentNumber = {}; // Stop the compiler complaining about initialization
//	mpf_init2(currentNumber, DECIMAL_ACCURACY);
//	mpf_set(currentNumber, numberToConvert);
//
//	mpf_t temp = {}; // Stop the compiler complaining about initialization
//	mpf_init2(temp, DECIMAL_ACCURACY);
//	mpf_set(temp, desiredBase);
//
//	// Continue while currentNumber is greater than 1
//	int i = floorOfBase;
//	int j = 0;
//	while (mpf_cmp_si(currentNumber, 1) > 0) {
//		while (mpf_cmp(currentNumber, mpf_pow_ui(temp, temp, temp)) > 0) {
//			i++;
//		}
//	i = floorOfBase;
//	j++;
//	}
//
//
//	// REWORK METHOD TO USE DIVISION INSTEAD. IT'S THE BEST WAY.
//	mpf_set(numberToConvert, currentNumber);
//}


int main(int argc, char **argv) {
	// Check if the user put in enough arguments
	if (argc != 4) {
		printf("Example usage: ChangeOfBase numberToConvert "
				       "desiredBase accuracy\n");
		printf("This only works for numbers between 0 and 1, "
				       "and bases between 1 and 36\n");
		return 0;
	}

	// Set values and print them to establish that the program is working as expected
	mpf_init2(numberToConvert, DECIMAL_ACCURACY);
	mpf_set_str(numberToConvert, argv[1], 10);
	gmp_printf("numberToConvert: %.Ff\n", numberToConvert);
	mpf_init2(desiredBase, DECIMAL_ACCURACY);
	mpf_set_str(desiredBase, argv[2], 10);
	gmp_printf("desiredBase: %.Ff\n", desiredBase);
	accuracy = (int) strtol((const char *) argv[3], NULL, 10);
	printf("accuracy: %d\n", accuracy);

	// Allocate memory for the string that represents the number in the new base
//	char *heapWhole = (char *) calloc((size_t) (accuracy + 1L), sizeof(char));
//	heapWhole[0] = '\0'; // No more trailing garbage
//	convertWholePartBaseToBase(heapWhole);
//	printf("conversion is %s", heapWhole);


	// Allocate memory for the string that represents the number in the new base
	char *heapFractional = (char *) calloc((size_t) (accuracy + 1L),
	                                       sizeof(char));
	heapFractional[accuracy] = '\0'; // No more trailing garbage
	convertFractionalPartBaseToBase(heapFractional);
	printf("conversion is .%s\n", heapFractional);

	return 0;
}
