#include <stdlib.h>
#define NUM_ITERS 64


// Complex scenario: heavy workload
int main(void)
{

    for (int i = 0; i < NUM_ITERS; i++) 
        free(malloc(i));

    for (int j = 0; j < NUM_ITERS; j++)
        realloc(calloc(j % 8, j / 8), 0);

    return 0;
}
