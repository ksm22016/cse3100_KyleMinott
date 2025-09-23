#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* print out an error message and exit */
void my_error(char *s)
{
    perror(s);
    exit(1);
}

/* Concatenate two strings into a newly allocated string. */
char *my_strcat(char *s1, char *s2)
{
    size_t n1 = strlen(s1);
    size_t n2 = strlen(s2);

    char *res = malloc(n1 + n2 + 1);   //
    if (!res) {
        my_error("malloc");
    }

    memcpy(res, s1, n1);
    memcpy(res + n1, s2, n2 + 1);      //Copy NUL too
    return res;
}

int main(int argc, char *argv[])
{
    char *s = my_strcat("", argv[0]);  //Start with program name

    for (int i = 1; i < argc; i++) {
        char *old = s;                 //Keep old to free after building new
        s = my_strcat(s, argv[i]);
        free(old);                     //Free previous concatenation
    }

    printf("%s\n", s);
    free(s);

    return 0;
}
