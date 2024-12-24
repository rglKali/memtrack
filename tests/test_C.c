#include <stdlib.h>

// Complex scenario: free the initial memory -- OK, as realloc returns the same address
int main(void)
{
    void * ptr = malloc(30);
    malloc(10);
    void * tmp = realloc(ptr, 40);
    free(ptr);
    return 0;
}
