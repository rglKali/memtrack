#ifndef _MEMTRACK_ENV_H_
#define _MEMTRACK_ENV_H_
#include <stddef.h>
#include <stdbool.h>
#include "cell.h"

typedef struct {
    _memtrack_cell_t * chunks;      // Array of memory cells
    size_t capacity;                // Capacity of the array
    size_t size;                    // Current size of the array
    bool initialized;               // Status of the environment -- true if initialized, false otherwise
    size_t calls;                   // Number of calls to the memory allocation functions
} _memtrack_env_t;

#endif  // _MEMTRACK_ENV_H_
