#include <stdlib.h>

// Calloc Zero memory
int main()
{
    void * ptr = calloc(0xf, 0);
    free(ptr);
    return 0;
}
