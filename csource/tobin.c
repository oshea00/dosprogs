#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void main() {
    double stoA = 2;    // input 
    double sto0 = 7;    // 10 exponent
    double sto1 = 128;  // base 2 digit
    double sto2 = 0;    // output
 
    double x = 0;

    printf("Enter base 10 integer: ");
    scanf("%lf", &stoA);

lbl1:
    if (sto0 < 0)
        goto lbl3;
    x = stoA;
    x -= sto1;
    if (x < 0)  
        goto lbl2;
    sto2 += pow(10,sto0);
    stoA -= sto1;
lbl2:
    sto1 /= 2;
    sto0 -= 1;
    goto lbl1;
lbl3:
    printf("sto2 = %.0f\n", sto2);
    return;
}