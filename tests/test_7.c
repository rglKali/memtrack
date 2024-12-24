#include <stdlib.h>

// Realloc Invalid ptr
int main()
{
    void * ptr = malloc(0xf);
    ptr--;
    realloc(ptr, 0);
    realloc("hello", 10);
    return 0;
}
