/*
*  Filename:      DecimalToBinary.c
*  Author:        Connor Baker
*  Version:       0.1c
*  Date Created:  January 20, 2016
*  Last Edited:   January 21, 2016
*  Description:   Convert a decimal entered by user into its binary
*                 representation. Currently limited to numbers smaller than
*                 18,446,744,073,709,551,615 (about 18 quintillion).
*  References:    http://codearea.in/dynamic-implementation-of-stack/ (for
*                 getting me started on how to create a reverse linked list
*                 (stack) in C)
*/

// Include the necessary header(s)
#include <stdio.h> // Standard input output
#include <stdlib.h> // Needed for malloc

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

// Create our stack/linked list
struct node {
  int data;
  struct node *link;
};

// Create a pointer to the top of the stack, and a temporary pointer
struct node *top, *temp;

// Querys the user for input
void getUserInput(void) {
  printf("%s\n", "Input the decimal you want to convert to binary");
  scanf("%llu", &decimal); // Grab the deciaml
  printf("%s\n", "You inputted:");
  printf("%llu\n", decimal); // Print back out the decimal to confirm accuracy
}

/*
*  Method to push the remainder to the top of the stack to be called for
*  printing.
*/
void push(int remainder)
{
  top = (struct node *)malloc(sizeof(struct node)); // Create the new node
  top->data = remainder; // Set the data portion of the node to the remainder
  top->link = temp; // Set the link to the previous node
  temp = top; // Set the temporary pointer to the top again
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
    push(tempRemainder); // Use the LUT
  }
}

/*
*  Method to print the binary representaiton stored in the stack. Starts at the
*  end of the linked list and traverses it backwards (each node has only a
*  bacwards facing pointer to the previous node).
*/
void display(void) {
  top = temp; // bring the top to top position
  printf("\n");
  while(top != NULL)
  {
    printf("%s", LUT[top->data]);
    top = top->link; // Now top points the previous node in the list
  }
  printf("\n");
}

// Main method just calls other methods
int main(void) {
  getUserInput();
  convertToBinary();
  display();
  return 0;
}
