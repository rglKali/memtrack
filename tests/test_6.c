#include <stdlib.h>

// Realloc Zero -- free-like behaviour
int main()
{
    void * ptr = malloc(0xf);
    void * tmp = realloc(ptr, 0);
    return 0;
}
