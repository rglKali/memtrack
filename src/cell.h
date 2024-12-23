#ifndef _MEMTRACK_CELL_H_
#define _MEMTRACK_CELL_H_
#include <stddef.h>
#include <stdbool.h>

typedef struct _memtrack_cell_s {
    void * address;     // Address of the allocated memory
    size_t size;        // Size of the allocated memory
    bool used;          // Status of the memory cell -- true for in used memory, false for free memory
} _memtrack_cell_t;

#endif  // _MEMTRACK_CELL_H_
