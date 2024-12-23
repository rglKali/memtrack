#ifndef _TRACK_04_H_
#define _TRACK_04_H_
#include <stddef.h>

void * _trace_malloc_f(size_t);
void _trace_free_f(void *);

#define malloc(size) _trace_malloc_f(size)
#define free(ptr) _trace_free_f(ptr)

#endif
