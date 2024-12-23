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
    for (size_t i = 0; i < _trace_env_c.size; i++) {
        if (!_trace_env_c.chunks[i].free) {
            fprintf(stderr, "WARNING: Missing free %p\n", _trace_env_c.chunks[i].address);
            free(_trace_env_c.chunks[i].address);
        }
    }
    free(_trace_env_c.chunks);
}

void _trace_setup_f() {
    if (!_trace_env_c.active) {
        _trace_initialise_f();
        atexit(_trace_release_f);
        _trace_env_c.active = true;
    }
}

void * _trace_malloc_f(size_t size) {
    if (!_trace_env_c.active) _trace_setup_f();
    void * ptr = malloc(size);
    for (size_t i = 0; i < _trace_env_c.size; i++) {
        if (_trace_env_c.chunks[i].address == ptr) {
            if (!_trace_env_c.chunks[i].free) {
                fprintf(stderr, "CRITICAL: Memory allocation overlap: %p\n", ptr);
                exit(1);
            }
            _trace_env_c.chunks[i].size += size;
            _trace_env_c.chunks[i].free = false;
            return ptr;
        }
    }
    _trace_env_c.chunks[_trace_env_c.size].address = ptr;
    _trace_env_c.chunks[_trace_env_c.size].size = size;
    _trace_env_c.chunks[_trace_env_c.size].free = false;
    _trace_env_c.size++;

    if (_trace_env_c.size == _trace_env_c.capacity) _trace_extend_f();

    return ptr;
}

void _trace_free_f(void * ptr) {
    if (!_trace_env_c.active) _trace_setup_f();
    if (ptr == NULL) {
        fprintf(stderr, "WARNING: Empty memory free\n");
        return;
    }
    for (size_t i = 0; i < _trace_env_c.size; i++) {
        if (_trace_env_c.chunks[i].address == ptr) {
            if (_trace_env_c.chunks[i].free) {
                fprintf(stderr, "ERROR: Double memory free: %p\n", ptr);
                return;
            }
            _trace_env_c.chunks[i].free = true;
            free(_trace_env_c.chunks[i].address);
            return;
        }
    }
    fprintf(stderr, "ERROR: Invalid memory free: %p\n", ptr);
    return;
}

int main(void)
{
    char *a=_trace_malloc_f(1);
    char *b=_trace_malloc_f(1);
    _trace_free_f(a);
    b++;
    _trace_free_f(b);
    return 0;
}
