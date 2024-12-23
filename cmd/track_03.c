#include "mtrack_03.h"


int main(void)
{
    char *a=_trace_malloc_f(1);
    char *b=_trace_malloc_f(1);
    _trace_free_f(a);
    b++;
    _trace_free_f(b);
    return 0;
}
