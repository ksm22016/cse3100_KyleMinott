#include <stdio.h>

/* This program should print the sum of the elements
 *     1^2, 2^2, 3^2, ..., n^2
 * where n is an integer provided by the user on the
 * command line. Hunt down the bugs and squash them!
 * Seek out the memory leaks and plug them up! */

/* Computes the sum of the first n elements in the array. */
int sum(int n, int* arr)
{
    int i;
    int total = 0;
    for (i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;                      //Return the value
}

/* Fills the given array with the values
 * 1^2, 2^2, 3^2, ..., (n-1)^2, n^2. */
void fillSquares(int n, int* arr)
{
    for (int i = 0; i < n; i++) {
        int k = i + 1;
        arr[i] = k * k;                //arr[0] = 1^2, ..., arr[n-1] = n^2
    }
}

/* Reads an integer n from arguments,
 * fills array with squares, and computes
 * the sum of the squares. Prints out the
 * sum before freeing all used memory. */
int main(int argc, char* argv[])
{
    // There must be at least 1 argument after the program
    // although only the first one is used.
    if (argc < 2) {
        printf("usage: ./squares n\n");
        return 1;
    }

    // convert the first argument from a string to a number
    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("n must be positive.\n");
        return 1;
    }

    int *arr = malloc((size_t)n * sizeof *arr);
    if (!arr) {
        perror("malloc");
        return 1;
    }

    fillSquares(n, arr);
    int total = sum(n, arr);
    printf("total: %d\n", total);

    free(arr);
    return 0;
}