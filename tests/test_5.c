#include <stdlib.h>

// Realloc Null -- malloc-like behaviour
int main()
{
    void * ptr = realloc(NULL, 0xf);
    free(ptr);
    return 0;
}
