#include <stdlib.h>
#include <stdio.h>
#include "cell.h"
#include "env.h"

// Define the initial size of the memory tracker underlying array
#define INITIAL_SIZE 16

// Extern the global placeholder
extern _memtrack_env_t _memtrack_env_c;

// Declare the internal functions
void _memtrack_release_f();

// Initialise the environment
void _memtrack_setup_f() {
    // Return if environment is already active
    if (_memtrack_env_c.initialized) return;

    // Set the default params
    _memtrack_env_c.capacity = INITIAL_SIZE;
    _memtrack_env_c.size = 0;

    // Allocate memory for the chunks
    _memtrack_cell_t * tmp = malloc(sizeof(_memtrack_cell_t) * _memtrack_env_c.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error allocating memory for the Environment\n");
        exit(1);
    }

    // Assign the chunks to the environment
    _memtrack_env_c.chunks = tmp;

    // Shedule the cleanup function
    atexit(_memtrack_release_f);

    // Mark the environemnt as active
    _memtrack_env_c.initialized = true;

    // Print the activation message
    fprintf(stderr, "\033[1;39m=== activate tracker ===\033[0m\n");
}
