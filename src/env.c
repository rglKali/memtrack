#include <stdlib.h>
#include <stdio.h>
#include "env.h"

void *  _memtrack_malloc_f  (char *, const char *, int, size_t);
void    _memtrack_free_f    (char *, const char *, int, void *);
void *  _memtrack_realloc_f (char *, const char *, int, void *, size_t);
void *  _memtrack_calloc_f  (char *, const char *, int, size_t, size_t);

#define malloc(size)        _memtrack_malloc_f(__FILE__, __func__, __LINE__, size)
#define free(ptr)           _memtrack_free_f(__FILE__, __func__, __LINE__, ptr)
#define realloc(ptr, size)  _memtrack_realloc_f(__FILE__, __func__, __LINE__, ptr, size)
#define calloc(nmemb, size) _memtrack_calloc_f(__FILE__, __func__, __LINE__, nmemb, size)

// Environment global placeholder
_memtrack_env_t _memtrack_env_c;
