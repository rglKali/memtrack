#include <stdlib.h>

// Complex scenario: free the initial memory -- not OK, as realloc returns new address
int main(void)
{
    void * ptr = malloc(30);
    malloc(10);
    void * tmp = realloc(ptr, 41);
    free(ptr);
    return 0;
}
