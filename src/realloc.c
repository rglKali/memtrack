#include <stdlib.h>
#include <stdio.h>
#include "cell.h"
#include "env.h"

// Extern the global placeholder
extern _memtrack_env_t _memtrack_env_c;

// Declare the internal functions
void _memtrack_setup_f();
void _memtrack_append_f(void * address, size_t size);

// Function to allocate memory
void * _memtrack_realloc_f(char * file, const char * func, int line, void * ptr, size_t size) {

    // Check if the environment is initialized
    if (!_memtrack_env_c.initialized) _memtrack_setup_f();

    // Increase the calls counter
    _memtrack_env_c.calls++;

    // Check if the pointer is NULL
    if (ptr == NULL) {

        // Reallocate the memory
        void * tmp = malloc(size);
        if (tmp == NULL) {
            fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | realloc(%p, %lu) -> NULL | CRITICAL: System error\033[0m\n",
                        _memtrack_env_c.calls, file, func, line, ptr, size);
            exit(1);
        }

        // Print the warning
        fprintf(stderr, "\033[1;33m[Call #%03lu] file<%s> | function<%s> | line<%d> | realloc(NULL, %lu) -> %p | WARNING: empty realloc\033[0m\n",
            _memtrack_env_c.calls, file, func, line, size, tmp);

        // Append the new cell to the storage
        _memtrack_append_f(tmp, size);

        // Return the new pointer
        return tmp;
    }

    // Search for the pointer in the storage
    for (size_t i = 0; i < _memtrack_env_c.size; i++) {
        if (_memtrack_env_c.chunks[i].address == ptr) {

            // Check if the address is in use
            if (_memtrack_env_c.chunks[i].used) {

                // Reallocate the memory
                void * tmp = realloc(ptr, size);
                if (ptr == NULL) {
                    fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | realloc(%p, %lu) -> NULL | CRITICAL: System error\033[0m\n",
                                _memtrack_env_c.calls, file, func, line, ptr, size);
                    exit(1);
                }

                // Mark cell as not used anymore
                _memtrack_env_c.chunks[i].used = false;

                // Print warning if size is 0
                if (size == 0) 
                    fprintf(stderr, "\033[1;33m[Call #%03lu] file<%s> | function<%s> | line<%d> | realloc(%p, %lu) -> %p | WARNING: Zero memory reallocation \033[0m\n",
                        _memtrack_env_c.calls, file, func, line, ptr, size, tmp);
                else {
                    // Append the new cell to the storage
                    _memtrack_append_f(tmp, size);
                    
                    // Print the common info message otherwise
                    fprintf(stderr, "\033[1;32m[Call #%03lu] file<%s> | function<%s> | line<%d> | realloc(%p, %lu) -> %p\033[0m\n",
                        _memtrack_env_c.calls, file, func, line, ptr, size, tmp);
                }   
                    
                // Return the new pointer
                return tmp;
            }
        }
    }

    // Actually, we shouldn't be here, if we reallocate the valid non-null ptr, so just throw an error
    fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | realloc(%p, %lu) -> NULL | ERROR: invalid pointer\033[0m\n",
            _memtrack_env_c.size, file, func, line, ptr, size);
    return NULL;
}
