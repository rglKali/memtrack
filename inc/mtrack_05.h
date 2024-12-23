#ifndef _TRACK_04_H_
#define _TRACK_04_H_
#include <stdlib.h>
#include <stddef.h>

void * _trace_malloc_f(char *, const char *, int, size_t);
void _trace_free_f(char *, const char *, int, void *);

#define malloc(size) _trace_malloc_f(__FILE__, __func__, __LINE__, size)
#define free(ptr) _trace_free_f(__FILE__, __func__, __LINE__, ptr)

#endif
