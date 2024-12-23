#include <stdlib.h>
#include <stdio.h>
#include "cell.h"
#include "env.h"

// Extern the global placeholder
extern _memtrack_env_t _memtrack_env_c;

// Declare the internal functions
void _memtrack_setup_f();

// Function to free the previously allocated memory
void _memtrack_free_f(char * file, const char * func, int line, void * ptr) {

    // Check if the environment is initialized
    if (!_memtrack_env_c.initialized) _memtrack_setup_f();

    // Increase the calls counter
    _memtrack_env_c.calls++;

    // Check if the pointer is NULL
    if (ptr == NULL) {

        // Print the error message
        fprintf(stderr, "\033[1;33m[Call #%03lu] file<%s> | function<%s> | line<%d> | free(NULL) | WARNING: empty free\033[0m\n",
            _memtrack_env_c.calls, file, func, line);

        // Return, as no action needed
        return;
    }

    // Search for the pointer in the storage
    for (size_t i = 0; i <_memtrack_env_c.size; i++) {
        if (_memtrack_env_c.chunks[i].address == ptr) {

            // Check if the address is in use
            if (_memtrack_env_c.chunks[i].used) {

                // Print the info message
                fprintf(stderr, "\033[1;32m[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p)\033[0m\n",
                        i+1, file, func, line, ptr);

                // Mark the memory as free
                _memtrack_env_c.chunks[i].used = false;

                // Free the memory
                free(_memtrack_env_c.chunks[i].address);

                // Return
                return;
            }

            // Print the error message
            fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p) | ERROR: double memory free\033[0m\n",
                    i+1, file, func, line, ptr);

            // Return
            return;
        }
    }

    // Print the error message
    fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p) | ERROR: invalid pointer\033[0m\n",
            _memtrack_env_c.size, file, func, line, ptr);

    // Return
    return;
}
