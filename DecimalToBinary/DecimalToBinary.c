/*
*  Filename:      DecimalToBinary16.c
*  Author:        Connor Baker
*  Version:       0.1a
*  Date Created:  January 20, 2016
*  Last Edited:   January 20, 2016
*  Description:   Convert a decimal entered by user into its binary
*                 representation. Currently limited to numbers smaller than
*                 18,446,744,073,709,551,615 (about 18 quintillion).
*/

// Include the necessary header(s)
#include <stdio.h> // Standard input output

// Declare our variables that we use throughout the duration of the program
unsigned long long decimal; // Holds the user-inputted decimal
unsigned int tempRemainder; // Holds the temporary remainder

/*
*  This is our look up table (LUT). It holds the binary representation of
*  all of our possible remainders.
*/
const char *LUT[16] = {"0000","0001","0010","0011",
                       "0100","0101","0110","0111",
                       "1000","1001","1010","1011",
                       "1100","1101","1110","1111"};

// Querys the user for input
void getUserInput(void) {
  printf("%s\n", "Input the decimal you want to convert to binary");
  scanf("%llu", &decimal); // Grab the deciaml
  printf("%s\n", "You inputted:");
  printf("%llu\n", decimal); // Print back out the decimal to confirm accuracy
}

// Converts the decimal to binary
void convertToBinary(void) {
  /*
  *  While the new quotient is not zero, continue to find binary
  *  representations for the remainders. If the new quotient is zero, stop
  *  (because that is our halt condition: at that point, we have found the full
  *  binary representation).
  */
  while (decimal != 0) {
    tempRemainder = decimal % 16; // Calculate the remainder
    decimal /= 16; // Calculate the quotient
    printf("%s\n", LUT[tempRemainder]); // Use the LUT
  }
}

int main(void) {
  getUserInput();
  convertToBinary();
  return 0;
}
