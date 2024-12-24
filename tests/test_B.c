#include <stdlib.h>

// Complex scenario: visually wrong code works properly
int main(void)
{
    void * ptr = malloc(30);
    free(ptr);
    malloc(40);     // malloc(40) returns the same address as ptr
    free(ptr);
    return 0;
}
