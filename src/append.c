#include <stdlib.h>
#include <stdio.h>
#include "cell.h"
#include "env.h"

// Extern the global placeholder
extern _memtrack_env_t _memtrack_env_c;

// Function to extend the array of memory cells
void _memtrack_append_f(void * address, size_t size) {
    // Address does not exist -> add it to the storage
    _memtrack_env_c.chunks[_memtrack_env_c.size].address = address;
    _memtrack_env_c.chunks[_memtrack_env_c.size].size = size;
    _memtrack_env_c.chunks[_memtrack_env_c.size].used = true;

    // Increase the array size
    _memtrack_env_c.size++;
}