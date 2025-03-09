#include "alloc.h"
#include <unistd.h>  // sbrk() (Use VirtualAlloc() for Windows)
#include <string.h>  // memcpy, memset
#include <stdio.h>   // Debugging output

// Global free list head
free_block *HEAD = NULL;
allocation_strategy current_strategy = FIRST_FIT;  // Default allocation strategy

// Requests memory from OS
void *do_alloc(size_t size) {
    free_block *block = sbrk(size + sizeof(free_block));
    if (block == (void *) -1) return NULL;  // Error handling

    block->size = size;
    block->next = NULL;
    block->magic = MAGIC;
    return (void *)(block + 1);
}

// Set allocation strategy
void set_allocation_strategy(allocation_strategy strategy) {
    current_strategy = strategy;
}

// Find a free block based on strategy
free_block *find_free_block(size_t size, free_block **prev) {
    free_block *best_block = NULL, *worst_block = NULL, *next_block = NULL;
    free_block *current = HEAD, *best_prev = NULL, *worst_prev = NULL, *next_prev = NULL;
    static free_block *last_allocated = NULL;  // For Next Fit

    if (current_strategy == FIRST_FIT) {
        while (current) {
            if (current->size >= size) {
                if (*prev) (*prev)->next = current->next;
                else HEAD = current->next;
                return current;
            }
            *prev = current;
            current = current->next;
        }
    }

    if (current_strategy == BEST_FIT) {
        while (current) {
            if (current->size >= size && (!best_block || current->size < best_block->size)) {
                best_block = current;
                best_prev = *prev;
            }
            *prev = current;
            current = current->next;
        }
        if (best_block) {
            if (best_prev) best_prev->next = best_block->next;
            else HEAD = best_block->next;
            return best_block;
        }
    }

    if (current_strategy == WORST_FIT) {
        while (current) {
            if (current->size >= size && (!worst_block || current->size > worst_block->size)) {
                worst_block = current;
                worst_prev = *prev;
            }
            *prev = current;
            current = current->next;
        }
        if (worst_block) {
            if (worst_prev) worst_prev->next = worst_block->next;
            else HEAD = worst_block->next;
            return worst_block;
        }
    }

    if (current_strategy == NEXT_FIT) {
        if (!last_allocated) last_allocated = HEAD;
        current = last_allocated;
        while (current) {
            if (current->size >= size) {
                last_allocated = current->next;  // Update for next search
                return current;
            }
            current = current->next;
        }
        last_allocated = HEAD;  // Restart search from beginning
    }

    return NULL;  // No suitable block found
}

// Custom malloc() implementation
void *tumalloc(size_t size) {
    if (size <= 0) return NULL;

    free_block *prev = NULL;
    free_block *block = find_free_block(size, &prev);

    if (block) {
        return (void *)(block + 1);
    }
    
    return do_alloc(size);
}

// Custom calloc() implementation
void *tucalloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void *ptr = tumalloc(total_size);
    if (ptr) memset(ptr, 0, total_size);
    return ptr;
}

// Custom realloc() implementation
void *turealloc(void *ptr, size_t new_size) {
    if (!ptr) return tumalloc(new_size);
    if (new_size == 0) {
        tufree(ptr);
        return NULL;
    }

    free_block *old_block = (free_block *)ptr - 1;
    if (old_block->size >= new_size) return ptr;

    void *new_ptr = tumalloc(new_size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_block->size);
        tufree(ptr);
    }
    return new_ptr;
}

// Custom free() implementation
void tufree(void *ptr) {
    if (!ptr) return;

    free_block *block = (free_block *)ptr - 1;
    if (block->magic != MAGIC) {
        printf("MEMORY CORRUPTION DETECTED\n");
        abort();
    }

    block->next = HEAD;
    HEAD = block;
}


