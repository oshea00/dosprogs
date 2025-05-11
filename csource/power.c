#include <stdio.h>
#include <math.h>  // Required for pow()

int main() {
    double base, exponent, result;

    // Input base and exponent
    printf("Enter base: ");
    scanf("%lf", &base);  // %lf for double input

    printf("Enter exponent: ");
    scanf("%lf", &exponent);  // %lf for double input

    // Calculate power
    result = pow(base, exponent);

    // Output result
    printf("%.2f raised to the power %.2f is %.4f\n", base, exponent, result);

    return 0;
}
