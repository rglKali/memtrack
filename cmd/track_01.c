#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#define INITIAL_SIZE 1

typedef struct {
    void * address;
    size_t size;
    bool free;
} Chunk;

struct Environment {
    Chunk * chunks;
    size_t capacity;
    size_t size;
} env;

void initialise() {
    env.capacity = INITIAL_SIZE;
    env.size = 0;
    Chunk * tmp = malloc(sizeof(Chunk) * env.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error allocating memory for the mAlloc Trace\n");
        exit(1);
    }
    env.chunks = tmp;
}

void extend() {
    env.capacity <<= 1;
    Chunk * tmp = realloc(env.chunks, sizeof(Chunk) * env.capacity);
    if (tmp == NULL) {
        fprintf(stderr, "CRITICAL: Error reallocating memory for the mAlloc Trace\n");
        exit(1);
    }
    env.chunks = tmp;
}

void destroy() {
    for (size_t i = 0; i < env.size; i++) {
        if (!env.chunks[i].free) {
            free(env.chunks[i].address);
        }
    }
    free(env.chunks);
}

void * mymalloc(size_t size) {
    void * ptr = malloc(size);
    for (size_t i = 0; i < env.size; i++) {
        if (env.chunks[i].address == ptr) {
            if (!env.chunks[i].free) {
                fprintf(stderr, "CRITICAL: Memory allocation overlap: %p\n", ptr);
                exit(1);
            }
            env.chunks[i].size += size;
            env.chunks[i].free = false;
            return ptr;
        }
    }
    env.chunks[env.size].address = ptr;
    env.chunks[env.size].size = size;
    env.chunks[env.size].free = false;
    env.size++;

    if (env.size == env.capacity) extend();

    return ptr;
}

void myfree(void * ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "WARNING: Empty memory free\n");
        return;
    }
    for (size_t i = 0; i < env.size; i++) {
        if (env.chunks[i].address == ptr) {
            if (env.chunks[i].free) {
                fprintf(stderr, "ERROR: Double memory free: %p\n", ptr);
                return;
            }
            env.chunks[i].free = true;
            free(env.chunks[i].address);
            return;
        }
    }
    fprintf(stderr, "ERROR: Invalid memory free: %p\n", ptr);
    return;
}

int main(void)
{
    initialise();
    char *a=mymalloc(1);
    char *b=mymalloc(1);
    myfree(a);
    b++;
    myfree(b);
    destroy();
    return 0;
}
