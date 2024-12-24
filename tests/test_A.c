#include <stdlib.h>

// Calloc with no free
int main()
{
    calloc(0xf, 0xf);
    return 0;
}
