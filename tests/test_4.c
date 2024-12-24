#include <stdlib.h>

// Invalid pointer free
int main()
{
    void * ptr = malloc(0xf);
    ptr++;
    free(ptr);
    free("hello");
    return 0;
}
