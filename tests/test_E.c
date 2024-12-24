#include <stdlib.h>

// Complex scenario: free the initial memory -- not OK, as realloc returns new address
int main(void)
{
    char ** ptr = calloc(30, sizeof(char));
    malloc(1);
    void * tmp = realloc(ptr, 31);
    free(ptr);
    realloc(tmp, 0);
    realloc(ptr, 0);
    return 0;
}
