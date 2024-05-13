#include "include/hash.h"
#include "include/utils.h"

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_HASH_CAPACITY 7
#define DEFAULT_LOAD_FACTOR 0.6


typedef struct map {
    /* callback on what the entry has to be compared against */
    bool (*compare_cb)(const void *, const void *, void *udata);
    /** callback on how to hash the entry */
    uint64_t (*hash_cb)(const void *, void *udata);
    /** unit size */
    size_t usize;
    /** capacity of hashmap */
    size_t cap;
    /** number of entries */
    size_t count;
    /** array of pointers that hold the data */
    void **entries;
} map_t;

int _object_insert(map_t *map, void **entries, size_t cap, void *unit);

void map_rehash(map_t *map, void **entries, size_t new_cap)
{
    for (size_t i = 0; i < map->cap; i++)
    {

        if (map->entries[i] != NULL)
        {
            _object_insert(map, entries, new_cap, map->entries[i]);
        }
    }
}

int map_realloc(map_t *map)
{
    size_t newSize = 0;

    if (map->count < (size_t)((map->cap * (1 - DEFAULT_LOAD_FACTOR)) * DEFAULT_LOAD_FACTOR))
    {
        newSize = (size_t)(map->cap * (double)(DEFAULT_LOAD_FACTOR));
    }
    else if (map->count >= (size_t)((double)map->cap * DEFAULT_LOAD_FACTOR))
    {
        newSize = (size_t)(map->cap * (double)(1 + DEFAULT_LOAD_FACTOR));
    }

    if (newSize == 0)
    {
        return 0;
    }

    newSize = GetHigher3mod4Prime(newSize);

    void *tmp = calloc(newSize, map->usize);
    if (tmp == NULL)
    {
        return -1;
    }

    // rehash, overwrite with new map->cap, free old entries, overwrite map entries pointer
    map_rehash(map, tmp, newSize);
    map->cap = newSize;
    free(map->entries);
    map->entries = tmp;

    return 0;
}

map_t *map_create(size_t unitSize, bool (*compare_cb)(const void *, const void *, void *udata),
                  uint64_t (*hash_cb)(const void *, void *udata))
{
    map_t *map = calloc(1, sizeof *map);
    if (NULL == map) {
        return NULL;
    }

    map->entries = calloc(7, sizeof(void *));
    if (NULL == map->entries) {
        free(map);
        return NULL;
    }

    map->cap = DEFAULT_HASH_CAPACITY;
    map->usize = unitSize;
    map->compare_cb = compare_cb;
    map->hash_cb = hash_cb;

    return map;
}


void map_destroy(map_t *map)
{
    free(map->entries);
    free(map);
}

void* map_get(map_t *map, void *unit)
{
    uint64_t hash = map->hash_cb(unit, NULL);

    for (size_t i = 0; i < map->cap; i++) {

        size_t table_index = hash_get_index(hash, i, map->cap);

        // printf("%zu\n", table_index);

        void **entry = &map->entries[table_index];

        if (NULL == *entry) {
            continue;
        } else {
            if (map->compare_cb(unit, *entry, NULL) == true) {
                return *entry;
            }
            else
            {
                continue;
            }
        }

    }

    return NULL;
}

int _object_insert(map_t *map, void **entries, size_t cap, void *unit)
{
    size_t table_index = 0;
    uint64_t hash = map->hash_cb(unit, NULL);

    for (size_t i = 0; i < cap; i++)
    {
        table_index = hash_get_index(hash, i, cap);

        if (entries[table_index] == NULL)
        {
            entries[table_index] = unit;
            return table_index;
        }

        if (map->compare_cb(entries[table_index], unit, NULL) == true)
        {
            continue;
        }
    }

    return -1;
}

int map_set(map_t *map, void *unit)
{
    uint64_t hash = map->hash_cb(unit, NULL);

    size_t index = -1;

    if ((index = _object_insert(map, map->entries, map->cap, unit)) == -1)
    {
        fprintf(stderr, "error: failed to insert object\n");
        return -1;
    }
    // printf("index: %zu\n", index);
    map->count++;
    map_realloc(map);

    return 0;
}

void *map_remove(map_t *map, void *unit)
{
    // printf("unit: %p\n", unit);
    uint64_t hash = map->hash_cb(unit, NULL);
    void *removed_unit = NULL;

    if (unit == NULL)
    {
        return NULL;
    }

    for (size_t i = 0; i < map->cap; i++)
    {
        size_t table_index = hash_get_index(hash, i, map->cap);

        void **entry = &map->entries[table_index];

        if (NULL == *entry)
        {
            continue;
        }
        else
        {
            /** comparison between found entry and value */
            if (map->compare_cb(unit, *entry, NULL) == true)
            {
                removed_unit = *entry;
                *entry = NULL;
                map->count--;
                map_realloc(map);
                // printf("Found and remove!\n");
                return removed_unit;
            }
        }
    }
    return NULL;
}

void map_scan(map_t *map, bool (*iter_cb)(const void *, void *))
{
    for (size_t i = 0; i < map->cap; i++) {
        if (map->entries[i] != NULL)
        {
            // printf("%zu:", i);
            iter_cb(map->entries[i], NULL);
        }
    }
}

int map_count(map_t *map)
{
    return map->count;
}

size_t map_cap(map_t *map)
{
    return map->cap;
}