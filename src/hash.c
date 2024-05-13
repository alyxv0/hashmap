#include "include/hash.h"

#include <stddef.h>
#include <inttypes.h>

#define FNV_OFFSET_BASE 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

uint64_t fnv1a_hash(uint64_t *hash, const void *mem, size_t len)
{
    uint64_t tmp_hash = FNV_OFFSET_BASE;
    uint8_t *p = (uint8_t *) mem;

    if (NULL == hash) {
        hash = &tmp_hash;
    }

    for (size_t i = 0; i < len; i++) {
        *hash = *hash ^ p[i];
        *hash = *hash * FNV_PRIME;
    }

    return *hash;
}

uint64_t fnv1_hash(uint64_t *hash, const void *mem, size_t len)
{
    uint64_t tmp_hash = FNV_OFFSET_BASE;
    uint8_t *p = (uint8_t*) mem;

    if (NULL == hash) {
        hash = &tmp_hash;
    }

    for (size_t i = 0; i < len; i++) {
        *hash = *hash * FNV_PRIME;
        *hash = *hash ^ p[i];
    }
    return *hash;
}

