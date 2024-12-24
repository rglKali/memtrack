#include <stdlib.h>

// Double free
int main(void)
{
    void * ptr = malloc(0xf);
    free(ptr);
    free(ptr);
    return 0;
}
