#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#define INITIAL_SIZE 16

typedef struct _trace_cell_s {
    void * address;
    size_t size;
    bool free;
} _trace_cell_t ;

struct _trace_env_s {
    _trace_cell_t * chunks;
    size_t capacity;
    size_t size;
    bool active;
} _trace_env_c;

void _trace_initialise_f() {
    _trace_env_c.capacity = INITIAL_SIZE;
    _trace_env_c.size = 0;
    _trace_cell_t * tmp = malloc(sizeof(_trace_cell_t) * _trace_env_c.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error allocating memory for the mAlloc Trace\n");
        exit(1);
    }
    _trace_env_c.chunks = tmp;
    fprintf(stdout, "=== activate tracker [v5] ===\n");
}

void _trace_extend_f() {
    _trace_env_c.capacity <<= 1;
    _trace_cell_t * tmp = realloc(_trace_env_c.chunks, sizeof(_trace_cell_t) * _trace_env_c.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error reallocating memory for the mAlloc Trace\n");
        exit(1);
    }
    _trace_env_c.chunks = tmp;
}

void _trace_release_f() {
    size_t unreleased = 0, released = 0;
    fprintf(stderr, "=== result tracker [v5] ===\n");
    for (size_t i = 0; i < _trace_env_c.size; i++) {
        if (!_trace_env_c.chunks[i].free) {
            fprintf(stderr, "Missing free(%p) -> %lub lost\n", _trace_env_c.chunks[i].address, _trace_env_c.chunks[i].size);
            free(_trace_env_c.chunks[i].address);
            unreleased += _trace_env_c.chunks[i].size;
        } else 
            released += _trace_env_c.chunks[i].size;
    }
    fprintf(stderr, "Total bytes allocated: %lub\n", unreleased + released);
    fprintf(stderr, "Total bytes released: %lub\n", released);
    fprintf(stderr, "Total bytes forgotten: %lub\n", unreleased);
    free(_trace_env_c.chunks);
}

void _trace_setup_f() {
    if (!_trace_env_c.active) {
        _trace_initialise_f();
        atexit(_trace_release_f);
        _trace_env_c.active = true;
    }
}

void * _trace_malloc_f(char * file, const char * func, int line, size_t size) {
    if (!_trace_env_c.active) _trace_setup_f();
    void * ptr = malloc(size);
    for (size_t i = 0; i < _trace_env_c.size; i++) {
        if (_trace_env_c.chunks[i].address == ptr) {
            if (!_trace_env_c.chunks[i].free) {
                fprintf(stderr, "CRITICAL: Memory allocation overleap\n");
                exit(1);
            }
            fprintf(stderr, "[Call #%03lu] file<%s> | function<%s> | line<%d> | malloc(%lu) -> %p\n",
                    _trace_env_c.size, file, func, line, size, ptr);
            _trace_env_c.chunks[i].size += size;
            _trace_env_c.chunks[i].free = false;
            return ptr;
        }
    }
    _trace_env_c.chunks[_trace_env_c.size].address = ptr;
    _trace_env_c.chunks[_trace_env_c.size].size = size;
    _trace_env_c.chunks[_trace_env_c.size].free = false;
    _trace_env_c.size++;

    fprintf(stderr, "[Call #%03lu] file<%s> | function<%s> | line<%d> | malloc(%lu) -> %p\n",
            _trace_env_c.size, file, func, line, size, ptr);
    if (_trace_env_c.size == _trace_env_c.capacity) _trace_extend_f();

    return ptr;
}

void _trace_free_f(char * file, const char * func, int line, void * ptr) {
    if (!_trace_env_c.active) _trace_setup_f();
    if (ptr == NULL) {
            fprintf(stderr, "[Call #000] file<%s> | function<%s> | line<%d> | free(NULL) -> ERROR: empty free\n",
                    file, func, line);
        return;
    }
    for (size_t i = 0; i < _trace_env_c.size; i++) {
        if (_trace_env_c.chunks[i].address == ptr) {
            if (_trace_env_c.chunks[i].free) {
                fprintf(stderr, "[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p) -> ERROR: double memory free\n",
                        i+1, file, func, line, ptr);
                return;
            }
            fprintf(stderr, "[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p)\n",
                    i+1, file, func, line, ptr);
            _trace_env_c.chunks[i].free = true;
            free(_trace_env_c.chunks[i].address);
            return;
        }
    }
    fprintf(stderr, "[Call #%03lu] file<%s> | function<%s> | line<%d> | free(%p) -> ERROR: invalid pointer\n",
            _trace_env_c.size, file, func, line, ptr);
    return;
}
