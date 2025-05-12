#include <stdio.h>

/**************************************************************************************************/
/* Calculates the number of bits set to 1 - or the Hamming Weight.                                */
/* See visualization here https://claude.ai/public/artifacts/4ac923e6-730e-466f-89f6-8ba85244ac09 */
/**************************************************************************************************/

int pop(unsigned x);

void main() {
    unsigned x = 0xF1001100;

    // Input base and exponent
    printf("Enter 32 bit unsigned number: ");
    scanf("%d", &x); 

    printf("pop(x) = %d 1-bits\n", pop(x));
}

int pop(unsigned x) {
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003f;
}