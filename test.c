#include <stdlib.h>
// #include "memtrack.h"

int main(void)
{
    int *a=malloc(0);
    a = realloc(a, 0);
    free(a);

    void *b = realloc(NULL, 4);
    void *c = realloc(b, 0);
    // char *b=malloc(1);
    // free(a);
    // free(NULL);
    // free(NULL);
    // free(a);
    // b++;
    // free(b);
    return 0;
}