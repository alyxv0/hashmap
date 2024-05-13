#ifndef HASHMAP_ALLOCATOR_H
#define HASHMAP_ALLOCATOR_H

#include <stddef.h>

typedef struct allocator {
    /** callback for allocator */
    int (*alloc_cb)(void *obj);
    /** callback for */
    int (*grow_cb)(void *obj);
    int (*shrink_cb)(void *obj);
    void *obj;
    double loadFactor;
    size_t usize;
    size_t count;
} allocator_t;

int allocate_test(allocator_t *allocator);

#endif