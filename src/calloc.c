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
void * _memtrack_calloc_f(char * file, const char * func, int line, size_t nmemb, size_t size) {

    // Check if the environment is initialized
    if (!_memtrack_env_c.initialized) _memtrack_setup_f();

    // Allocate the memory
    void * ptr = calloc(nmemb, size);
    if (ptr == NULL) {
        fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | calloc(%lu, %lu) -> NULL | CRITICAL: System error\033[0m\n",
                    _memtrack_env_c.calls, file, func, line, nmemb, size);
        exit(1);
    }

    // Add the cell to the array
    _memtrack_append_f(ptr, nmemb * size);

    // Print warning is size is 0
    if (size == 0) 
        fprintf(stderr, "\033[1;33m[Call #%03lu] file<%s> | function<%s> | line<%d> | calloc(%lu, %lu) -> %p | WARNING: Zero memory allocation \033[0m\n",
            _memtrack_env_c.calls, file, func, line, nmemb, size, ptr);
    // Print warning if allocating 0 members
    else if (nmemb == 0)
        fprintf(stderr, "\033[1;33m[Call #%03lu] file<%s> | function<%s> | line<%d> | calloc(%lu, %lu) -> %p | WARNING: Zero members allocation \033[0m\n",
            _memtrack_env_c.calls, file, func, line, nmemb, size, ptr);
    // Print the common info message otherwise
    else 
        fprintf(stderr, "\033[1;32m[Call #%03lu] file<%s> | function<%s> | line<%d> | calloc(%lu, %lu) -> %p\033[0m\n",
            _memtrack_env_c.calls, file, func, line, nmemb, size, ptr);

    // Return the pointer
    return ptr;

}
