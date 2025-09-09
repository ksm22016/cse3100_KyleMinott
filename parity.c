#include <stdio.h>

int main(void) {
    unsigned int v = 19;
    char parity = 0;     
    while (v) {
        parity = !parity;
        v = v & (v - 1);
    }

    printf("%d\n", parity); //1 if odd count of 1s, else 0
    return 0;
}