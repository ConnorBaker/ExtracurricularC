/*
 *  Filename:      FindPrimes.c
 *  Author:        Connor Baker
 *  Version:       0.1a
 *  Date Created:  August 14, 2017
 *  Last Edited:   August 14, 2017
 *  Description:   Finds and prints primes less than the number inputted by
 *                 the user.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Declare all of our functions
void find_primes(int upper_bound, bool *is_prime);
// int get_int_from_cli_unsafe(void);
bool get_int_from_cli_safe(int *num);

int main(void);

// Find primes given an integer array of size greater than 2
void find_primes(int upper_bound, bool *is_prime) {
    // Fill our array for processing
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
}

bool get_int_from_cli_safe(int *num) {
    char input_string[12]; // signed ints are at most 10 characters, we go two over since the first and last are tossed
    char *pointer_to_remainder_of_string;
    errno = 0; // Set to zero to check later if strtol had an issue parsing the input
    if (fgets(input_string, sizeof(input_string), stdin)) { // check that we can open our input
        long li = strtol(input_string, &pointer_to_remainder_of_string, 10);
        if (ERANGE == errno) {
            fprintf(stderr, "error: Number is out of the range of a long data type.\n");
            return false;
        }
        if (li > INT_MAX || li < INT_MIN) {
            fprintf(stderr, "error: %li is out of the range of a signed int data type.\n", li);
            return false;
        }
        if (pointer_to_remainder_of_string == input_string) {
            fprintf(stderr, "error: Your input '%.*s' is not valid numerical input.\n",
                    (int) strlen(input_string) - 1, input_string);
            return false;
        }

        // If none of the above are triggered, we are good to go to convert our long to an int
        *num = (int) li;
        return true;
    } else { // return an error if we were unable to use fgets
        fprintf(stderr, "error: Could not open input from stdin.\n");
        return false;
    }
}

int main() {
    printf("Please enter an integer between 2 and %d\n", INT_MAX - 1);
    int upper_bound = 0;
    while (!get_int_from_cli_safe(&upper_bound)) {
        fprintf(stderr, "Unable to parse integer from console.\n");
        printf("Please enter an integer between 2 and 100\n");
    }

    printf("You entered: %d\n", upper_bound);
    printf("Finding all primes in [2,%d]\n", upper_bound);

    // Allocate memory for our array of booleans
    bool *is_prime;
    is_prime = malloc((size_t) (upper_bound + 1)); // a bool is size 1

    // Check that we were able to create the array
    if (!is_prime) {
        fprintf(stderr, "There was a problem allocating memory for the array.\n");
        abort();
    }

    // Invoke our function to find the primes in the passed array
    find_primes(upper_bound, is_prime);

    // Print out array primes
    for (int i = 2; i < upper_bound + 1; ++i) {
        if (is_prime[i] != 0) {
            printf("%d\n", i);
        }
    }

    // Free the memory that we allocated for our array
    free(is_prime);
}
