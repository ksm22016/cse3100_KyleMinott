#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;
    printf("n = ");
    scanf("%d", &n);

    double pi = 0.0;
    double p = 1.0;  // represents 1/(16^i)

    for (i = 0; i <= n; i++) {
        pi += ( (4.0/(8*i+1)) - (2.0/(8*i+4)) - (1.0/(8*i+5)) - (1.0/(8*i+6)) ) * p;
        p /= 16.0;  // update p for next term
    }

    printf("PI = %.10f\n", pi);
    return 0;
}
