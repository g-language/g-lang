#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

void* mem_new(const size_t size) {
    return calloc(1, size);
}

void* mem_free(const void* mem) {
    if (mem) free((void*)mem);
    return NULL;
}
