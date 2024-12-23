#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#define INITIAL_SIZE 16

typedef struct _memtrack_cell_s {
    void * address;
    size_t size;
    bool free;
} _memtrack_cell_t;

struct _memtrack_env_s {
    _memtrack_cell_t * chunks;
    size_t capacity;
    size_t size;
    bool active;
} _memtrack_env_c;

void _memtrack_initialise_f() {
    _memtrack_env_c.capacity = INITIAL_SIZE;
    _memtrack_env_c.size = 0;
    _memtrack_cell_t * tmp = malloc(sizeof(_memtrack_cell_t) * _memtrack_env_c.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error allocating memory for the mAlloc Trace\n");
        exit(1);
    }
    _memtrack_env_c.chunks = tmp;
    fprintf(stdout, "\033[1;39m=== activate tracker [v5] ===\033[0m\n");
}

void _memtrack_extend_f() {
    _memtrack_env_c.capacity <<= 1;
    _memtrack_cell_t * tmp = realloc(_memtrack_env_c.chunks, sizeof(_memtrack_cell_t) * _memtrack_env_c.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error reallocating memory for the mAlloc Trace\n");
        exit(1);
    }
    _memtrack_env_c.chunks = tmp;
}

void _memtrack_release_f() {
    size_t unreleased = 0, released = 0;
    fprintf(stderr, "\033[1;39m=== result tracker [v5] ===\033[0m\n");
    for (size_t i = 0; i < _memtrack_env_c.size; i++) {
        if (!_memtrack_env_c.chunks[i].free) {
            fprintf(stderr, "\033[1;31mMissing free(%p) -> %lub lost\033[0m\n", _memtrack_env_c.chunks[i].address, _memtrack_env_c.chunks[i].size);
            free(_memtrack_env_c.chunks[i].address);
            unreleased += _memtrack_env_c.chunks[i].size;
        } else 
            released += _memtrack_env_c.chunks[i].size;
    }
    fprintf(stderr, "\033[1;39mTotal bytes allocated: %lub\033[0m\n", unreleased + released);
    fprintf(stderr, "\033[1;39mTotal bytes released: %lub\033[0m\n", released);
    fprintf(stderr, "\033[1;39mTotal bytes forgotten: %lub\033[0m\n", unreleased);
    free(_memtrack_env_c.chunks);
}

void _memtrack_setup_f() {
    if (!_memtrack_env_c.active) {
        _memtrack_initialise_f();
        atexit(_memtrack_release_f);
        _memtrack_env_c.active = true;
    }
}

void * _memtrack_malloc_f(char * file, const char * func, int line, size_t size) {
    if (!_memtrack_env_c.active) _memtrack_setup_f();
    void * ptr = malloc(size);
    for (size_t i = 0; i < _memtrack_env_c.size; i++) {
        if (_memtrack_env_c.chunks[i].address == ptr) {
            if (!_memtrack_env_c.chunks[i].free) {
                fprintf(stderr, "CRITICAL: Memory allocation overleap\n");
                exit(1);
            }
            fprintf(stderr, "\033[1;32m[Call #%03lu] file<%s> | function<%s> | line<%d> | malloc(%lu) -> %p\033[0m\n",
                    _memtrack_env_c.size, file, func, line, size, ptr);
            _memtrack_env_c.chunks[i].size += size;
            _memtrack_env_c.chunks[i].free = false;
            return ptr;
        }
    }
    _memtrack_env_c.chunks[_memtrack_env_c.size].address = ptr;
    _memtrack_env_c.chunks[_memtrack_env_c.size].size = size;
    _memtrack_env_c.chunks[_memtrack_env_c.size].free = false;
    _memtrack_env_c.size++;

    fprintf(stderr, "\033[1;32m[Call #%03lu] file<%s> | function<%s> | line<%d> | malloc(%lu) -> %p\033[0m\n",
            _memtrack_env_c.size, file, func, line, size, ptr);
    if (_memtrack_env_c.size == _memtrack_env_c.capacity) _memtrack_extend_f();

    return ptr;
}

void _memtrack_free_f(char * file, const char * func, int line, void * ptr) {
    if (!_memtrack_env_c.active) _memtrack_setup_f();
    if (ptr == NULL) {
            fprintf(stderr, "\033[1;31m[Call #000] file<%s> | function<%s> | line<%d> | free(NULL) -> ERROR: empty free\033[0m\n",
                    file, func, line);
        return;
    }
    for (size_t i = 0; i < _memtrack_env_c.size; i++) {
        if (_memtrack_env_c.chunks[i].address == ptr) {
            if (_memtrack_env_c.chunks[i].free) {
                fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p) -> ERROR: double memory free\033[0m\n",
                        i+1, file, func, line, ptr);
                return;
            }
            fprintf(stderr, "\033[1;32m[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p)\033[0m\n",
                    i+1, file, func, line, ptr);
            _memtrack_env_c.chunks[i].free = true;
            free(_memtrack_env_c.chunks[i].address);
            return;
        }
    }
    fprintf(stderr, "\033[1;31m[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p) -> ERROR: invalid pointer\033[0m\n",
            _memtrack_env_c.size, file, func, line, ptr);
    return;
}

void _memtrack_realloc_f(char * file, const char * func, int line, void * ptr, size_t size) {

}

void _memtrack_calloc_f(char * file, const char * func, int line, size_t nmemb, size_t size) {

}
