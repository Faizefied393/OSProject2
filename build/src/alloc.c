#include "alloc.h"
#include <windows.h>  // For VirtualAlloc
#include <string.h>   // For memset, memcpy
#include <stdio.h>    // For debugging

#define ALIGNMENT 16  // Memory alignment

static free_block *HEAD = NULL;  // Free list head

// Windows equivalent of sbrk
void *do_alloc(size_t size) {
    free_block *block = VirtualAlloc(NULL, size + sizeof(free_block), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!block) return NULL;  // Error handling

    block->size = size;
    block->next = NULL;
    return (void *)(block);
}
