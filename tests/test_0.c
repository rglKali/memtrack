#include <stdlib.h>

// Zero malloc
int main()
{
    void * ptr = malloc(0);
    free(ptr);
    return 0;
}