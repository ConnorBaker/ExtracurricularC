// Author: Connor Baker
// Created: November 14, 2017
// Version: 0.3a

/* TODO: Determine at runtime the size of the whole part representation.
 * Should be no larger than the first power of the base larger than the number.
 */

// Import headers
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>



// We should be able to derive this value from the accuracy of the base inputted
#define DECIMAL_ACCURACY 32768



// Define variable we use throughout
mpf_t numberToConvert;
mpf_t desiredBase;
int accuracy;



// Create a dictionary to pull from for any alphabet we need, up to base 36
const char dictionary[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                           'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                           'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                           'U', 'V', 'W', 'X', 'Y', 'Z'};


// Declare our functions before use
unsigned long funcOne(mpf_t num);

unsigned long funcTwo(mpf_t num, unsigned long exponent);

void convertWholePartBaseToBase(char *heap);

void convertFractionalPartBaseToBase(char *heap);

int main(int argc, char *argv[]);



/*
 * ABOUT:
 * Given a number, find the largest possible power of the desiredBase that we can
 * take away from it.
 *
 * ALGORITHM:
 * Starting from desiredBase^0, and incrementing power by one, find the largest
 * power of the base that is less than the number passed in.
 *
 * RETURNS:
 * An unsigned long representing the largest possible power of the desiredBase that can
 * be taken away from the number passed in.
 */
unsigned long funcOne(mpf_t num) {
	unsigned long i = 0;

	mpf_t temp = {}; // Stop the compiler complaining about initialization
	mpf_init2(temp, DECIMAL_ACCURACY);
	mpf_pow_ui(temp, desiredBase, i);

	// While the power we have is greater than or equal to the number passed in,
	// continue to up the exponent. When we fail to meet this condition, we've gone
	// too far and must return one less than that exponent
	while (mpf_cmp(num, temp) > 0) {
		i++;
		mpf_pow_ui(temp, desiredBase, i);
	}

	return (i-1);
}



/*
 * ABOUT:
 * Given a number, and the largest possible power of a base, find the largest multiple
 * of the power of the base that is less than the number.
 *
 * ALGORITHM:
 * Starting from 0*(desiredBase^exponent), and incrementing coefficient by one, find the
 * largest power of the base that is less than the number passed in.
 *
 * RETURNS:
 * An unsigned long representing the largest possible coefficient of the desiredBase to
 * the exponent passed in that can be taken away from the number passed in.
 */
unsigned long funcTwo(mpf_t num, unsigned long exponent) {
	unsigned long i = 0;

	mpf_t temp = {}; // Stop the compiler complaining about initialization
	mpf_init2(temp, DECIMAL_ACCURACY);

	mpf_pow_ui(temp, desiredBase, exponent);
	mpf_mul_ui(temp, temp, i);

	// While the coefficient of the power of the base we have is greater than or equal
	// to the number passed in, continue to up the coefficient. When we fail to meet
	// this condition, we've gone too far and must return one less than that coefficient
	while(mpf_cmp(num, temp) > 0) {
		i++;
		mpf_pow_ui(temp, desiredBase, exponent);
		mpf_mul_ui(temp, temp, i);
	}

	return (i-1);
}



/*
 * ABOUT:
 * This method converts the whole portion of a number to a different base.
 *
 * ALGORITHM:
 * Toss the coefficient of the greatest power of the base that we can take away into
 * the heap. Then, take it away from currentNumber (which starts out as the number to be
 * converted). Rinse and repeat, until it's less than or equal to one. Then, pass to the
 * convertFractionalPartBaseToBase() function.
 */
void convertWholePartBaseToBase(char *heap) {
	mpf_t currentNumber = {}; // Stop the compiler complaining about initialization
	mpf_init2(currentNumber, DECIMAL_ACCURACY);
	mpf_set(currentNumber, numberToConvert);

	mpf_t temp = {}; // Stop the compiler complaining about initialization
	mpf_init2(temp, DECIMAL_ACCURACY);

	// Largest removable base and coefficient, and index
	unsigned long powerOfBase = 0;
	unsigned long previousPow = 0;
	unsigned long coeffOfPowerOfBase = 0;
	unsigned long index = 0;

	while (mpf_cmp_ui(currentNumber, 1) >= 0) {
		// Update previousPow
		previousPow = powerOfBase;

		// Calculate the largest power of the base we can remove
		powerOfBase = funcOne(currentNumber);

		// Calculate the largest multiple of the exponentiated base we can remove
		coeffOfPowerOfBase = funcTwo(currentNumber, powerOfBase);

		// Debugging
//		printf("powerOfBase = %lu\n", powerOfBase);
//		printf("previousPow = %lu\n", previousPow);
//		printf("coeffOfPowerOfBase = %lu\n", coeffOfPowerOfBase);
//		printf("index = %lu\n", index);

		// Catch the difference between the powers and fill with zeros
		while (previousPow > (powerOfBase + 1L)) {
//			printf("'0' at index %lu\n", index);
			heap[index++] = '0';
//			printf("index should be one greater now: %lu\n", index);
			previousPow--;
		}

		// Debugging
//		printf("End add zero loop\n");

		// Take calculated number away from currentNumber
		mpf_pow_ui(temp, desiredBase, powerOfBase);
		mpf_mul_ui(temp, temp, coeffOfPowerOfBase);
		mpf_sub(currentNumber, currentNumber, temp);

		// Add the coefficient to the heap
//		printf("'%lu' at index %lu\n", coeffOfPowerOfBase, index);
		heap[index++] = dictionary[coeffOfPowerOfBase];
//		printf("index should be one greater now: %lu\n", index);

		// Debugging
//		printf("End of main while loop\n");
	}

	// Catch the case that the number is very nearly a power of the base
	// and add zeros to fill in the lower powers of the base that were skipped over.
	// Ex: 16 in base two is 10000
	// The main while loop only checks that we have some greater-than-zero part of
	// the number left, so we don't get to loop through a second time to fill in the
	// missing (which is indicated by the difference between powerOfBase and
	// previousPow)
	while (powerOfBase > 0L) {
//		printf("'0' at index %lu\n", index);
		heap[index++] = '0';
//		printf("index should be one greater now: %lu\n", index);
		powerOfBase--;
	}

	// Pass the remainder over to our other method
	mpf_set(numberToConvert, currentNumber);
}



/*
 * ABOUT:
 * This method converts the fractional portion of a number to a different base.
 * It should rarely be called directly -- instead, it should be called by the
 * convertWholePartBaseToBase() function, which passes its leftovers on.
 *
 * ALGORITHM:
 * By repeatedly multiplying our number to convert to a different base by the desired
 * base, and then chopping off the whole part of the product, we can derive the
 * representation in the desired base. It's a bit like the greedy algorithm for
 * traditional change of base, but in reverse -- at least in the sense that we are
 * multiplying, and are keeping the whole part of the product, instead of the remainder.
 *
 * Formally, this is sometimes called the repeated application of the Beta Map. By taking
 * a positive number less than one and multiplying it by the desired base, we can imagine
 * stretching the unit interval to some length that is exactly the desired base length
 * long. By chopping off the whole number part (usually performed by doing modulo one in
 * a pure mathematics context), we shift everything back to the unit interval.
 */
void convertFractionalPartBaseToBase(char *heap) {
	mpf_t currentNumber = {}; // Stop the compiler complaining about initialization
	mpf_init2(currentNumber, DECIMAL_ACCURACY);

	// We keep track of the whole part of the product.
	char wholePartOfProduct = 0;

	// Make sure we remove the whole part of the number before proceeding
	// Just in case we have leftovers for some reason from the previous function
	mpf_sub_ui(currentNumber, numberToConvert, mpf_get_ui(numberToConvert));

	for (int i = 0; i < accuracy; i++) {
		mpf_mul(currentNumber, currentNumber, desiredBase);
		wholePartOfProduct = (char) mpf_get_d(currentNumber);
		heap[i] = dictionary[wholePartOfProduct];
		mpf_sub_ui(currentNumber, currentNumber,
		           (unsigned long) wholePartOfProduct);
	}
}



int main(int argc, char **argv) {
	// Check if the user put in enough arguments
	if (argc != 4) {
		printf("Example usage: ChangeOfBase numberToConvert "
				       "desiredBase accuracy\n");
		printf("This only works for positive numbers, "
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
	char *heapWhole = (char *) calloc((size_t) (accuracy + 1L), sizeof(char));
	heapWhole[0] = '\0'; // No more trailing garbage
	convertWholePartBaseToBase(heapWhole);

	// Allocate memory for the string that represents the number in the new base
	char *heapFractional = (char *) calloc((size_t) (accuracy + 1L),
	                                       sizeof(char));
	heapFractional[accuracy] = '\0'; // No more trailing garbage
	convertFractionalPartBaseToBase(heapFractional);
	printf("conversion is %s.%s\n", heapWhole, heapFractional);

	return 0;
}
