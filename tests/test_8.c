#include <stdlib.h>

// Calloc Zero members
int main()
{
    void * ptr = calloc(0, 0xf);
    free(ptr);
    return 0;
}
