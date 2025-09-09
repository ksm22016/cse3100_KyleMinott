#include <stdio.h>

int main(void) {
    double x;
    double total = 0.0;
    long long count = 0;   // count of numbers read

    // Read until scanf fails (error or EOF)
    while (scanf("%lf", &x) == 1) {
        total += x;
        count++;
        double average = total / (double)count;
        printf("Total=%f Average=%f\n", total, average);
    }

    return 0;
}