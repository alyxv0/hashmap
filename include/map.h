#ifndef HASHMAP_MAP_H
#define HASHMAP_MAP_H

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>



uint64_t fnv1a_hash(uint64_t *hash, const void *mem, size_t len);
uint64_t fnv1_hash(uint64_t *hash, const void *mem, size_t len);

typedef struct map map_t;
int map_realloc(map_t *map);

map_t *map_create(size_t unitSize, bool (*compare_cb)(const void *, const void *, void *),
                  uint64_t (*hash_cb)(const void *));

void map_destroy(map_t *map);

void* map_get(map_t *map, void *unit);
void *map_set(map_t *map, void *unit);
int map_remove(map_t *map, void *unit);

void map_scan(map_t *map, bool (*iter_cb)(const void *, void *));
size_t map_count(map_t *map);
size_t map_cap(map_t *map);

#endif