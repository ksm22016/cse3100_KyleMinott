#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    printf("n = ");
    scanf("%d", &n);

    int m = n;  // keep original number

    while (n != 1 && n != 4) {
        int sum = 0;
        int temp = n;
        while (temp > 0) {
            int digit = temp % 10;
            sum += digit * digit;
            temp /= 10;
        }
        n = sum;
        printf("%d\n", n);  // print intermediate number
    }

    if (n == 1)
        printf("%d is a happy number.\n", m);
    else
        printf("%d is a NOT a happy number.\n", m);

    return 0;
}
