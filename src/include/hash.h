#ifndef HASHMAP_HASH_H
#define HASHMAP_HASH_H

#include <stddef.h>
#include <inttypes.h>

uint64_t fnv1a_hash(uint64_t *hash, const void *mem, size_t len);
uint64_t fnv1_hash(uint64_t *hash, const void *mem, size_t len);

#endif