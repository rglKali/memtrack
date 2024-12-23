#include <stdio.h>
#include <stdlib.h>
// #include "memtrack.h"

int main(void)
{

    int *a = calloc(4, 4);
    int *b = calloc(0, 4);
    int *c = calloc(4, 0);
    int *d = calloc(0, 0);
    printf("%p, %p, %p, %p\n", a, b, c, d);
    free(a);
    free(b);
    free(c);
    free(d);
    // int *a=malloc(0);
    // // printf("%p\n", a);
    // free(a);
    // a = realloc(a, 0);
    // free(a);

    // void *b = realloc(NULL, 4);
    // void *c = realloc(b, 0);
    // char *b=malloc(1);
    // free(a);
    // free(NULL);
    // free(NULL);
    // free(a);
    // b++;
    // free(b);
    return 0;
}