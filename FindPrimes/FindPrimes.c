/*
 *  Filename:      FindPrimes.c
 *  Author:        Connor Baker
 *  Version:       0.1b
 *  Date Created:  August 14, 2017
 *  Last Edited:   August 15, 2017
 *  Description:   Finds and prints primes less than the number inputted by
 *                 the user.
 *  Todo:          Make a function for printing the array.
 *  Todo:          Make it a true tool and accept arguments passed via cli.
 */

// Declare header imports
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Declare all of our functions in order of appearance
bool* find_primes(int upper_bound);
bool get_int_from_cli_safe(int *num);

// Declare the main method
int main(void);

// Find primes given an integer array of size greater than 2
bool* find_primes(int upper_bound) {

  // Allocate memory to our new array. Since sizeof(bool) is one, we can
  // omit multiplying by it inside of the malloc since it won't change
  // the product
  bool *is_prime = malloc((size_t) (upper_bound + 1));

  // Check that we were able to create the array
  if (!is_prime) {
      fprintf(stderr, "There was a problem allocating memory for the array.\n");
      abort();
  }
    // Fill our array for processing -- numbers are prime unless found otherwise
    for (int i = 0; i < upper_bound + 1; ++i) {
        is_prime[i] = true;
    }

    // Remove all multiples of two except for 2^1
    for (int i = 4; i < upper_bound + 1; i += 2) {
        is_prime[i] = false;
    }

    // Remove all multiples of other non-eliminated numbers
    for (int i = 3; i <= (int) (sqrt(upper_bound + 1)); i += 2) {
        for (int j = i * i; j < upper_bound + 1; j += 2 * i) {
            // Multiply by 2 to remove first multiple of the prime, but not prime itself
            is_prime[j] = false;
        }
    }

    // Return the address of the array. The caller knows the size, so we don't
    // need to return it as a tuple
    return is_prime;
}


/*
*  I use strtol here instead of scanf because it's very good for warning about
*  errors. In fact, if the user decides to input something that's out of the
*  range of a long, then it certainly won't fit inside of an int.
*
*  Checks performed include:
*  - Buffer overflow (the ERANGE == errno check)
*  - Type narrowing
*  - Check for valid numerical input (ex. being 'six' vs. 6)
*/
bool get_int_from_cli_safe(int *num) { // Perform rigorous error checking
    char input_string[12]; // signed ints are at most 10 characters, we go two over since the first and last are tossed
    char *pointer_to_remainder_of_string;
    errno = 0; // Set to zero to check later if strtol had an issue parsing the input
    if (fgets(input_string, sizeof(input_string), stdin)) { // check that we can open our input
        long li = strtol(input_string, &pointer_to_remainder_of_string, 10);
        if (ERANGE == errno) {
            fprintf(stderr, "error: Number is out of the range of a long data type.\n");
            return false; // Signal to the caller that there was a problem
        }
        if (li > INT_MAX || li < INT_MIN) {
            fprintf(stderr, "error: %li is out of the range of a signed int data type.\n", li);
            return false;  // Signal to the caller that there was a problem
        }
        if (pointer_to_remainder_of_string == input_string) {
            fprintf(stderr, "error: Your input '%.*s' is not valid numerical input.\n",
                    (int) strlen(input_string) - 1, input_string);
            return false;  // Signal to the caller that there was a problem
        }

        // If none of the above are triggered, we are good to go
        // to convert our long to an int
        *num = (int) li;
        return true;  // Signal to the caller that we were successful
    } else { // return an error if we were unable to use fgets
        fprintf(stderr, "error: Could not open input from stdin.\n");
        return false;  // Signal to the caller that there was a problem
    }
}

int main() {
    printf("Please enter an integer between 2 and %d\n", INT_MAX - 1);
    int upper_bound = 0;

    // Prompt the user until we get what we want from them
    while (!get_int_from_cli_safe(&upper_bound)) {
        fprintf(stderr, "Unable to parse integer from console.\n");
        printf("Please enter an integer between 2 and 100\n");
    }

    // Print back out what the user inputted
    printf("You entered: %d\n", upper_bound);
    printf("Finding all primes in [2,%d]\n", upper_bound);

    // Invoke our function to mark primes in the passed boolean array
    bool *primes = find_primes(upper_bound);

    // Print out the numbers we found to be prime
    printf("(2");
    for (int i = 3; i < upper_bound + 1; ++i) {
        if(primes[i] != 0) {
          printf(", %d", i);
        }
    }
    printf(")\n");

    // Free the memory that we allocated for our array since it was not static
    free(primes);
}
