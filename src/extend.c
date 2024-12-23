#include <stdlib.h>
#include <stdio.h>
#include "cell.h"
#include "env.h"

// Extern the global placeholder
extern _memtrack_env_t _memtrack_env_c;

// Function to extend the array of memory cells
void _memtrack_extend_f() {

    // Extend the capacity of the memory tracker
    _memtrack_env_c.capacity <<= 1;

    // Reallocate the chunks array
    _memtrack_cell_t * tmp = realloc(_memtrack_env_c.chunks, sizeof(_memtrack_cell_t) * _memtrack_env_c.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error reallocating memory for the mAlloc Trace\n");
        exit(1);
    }

    // Update the chunks array
    _memtrack_env_c.chunks = tmp;
}
