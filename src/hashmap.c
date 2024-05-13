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

void map_rehash(void **old_entries, size_t old_size, void **new_entries, size_t new_size) {
    for (size_t i = 0; i < old_size; i++) {
        if (old_entries[old_size] != NULL) {

        }
    }
}

int map_realloc(map_t *map)
{
    size_t newSize = 0;
    if (map->count >= (size_t)(map->cap * DEFAULT_LOAD_FACTOR)) {
        newSize = (size_t) (map->cap * (double)(1 + DEFAULT_LOAD_FACTOR));
    } else {
        return -1;
    }
    printf("1 New Size: %zu\n", newSize);
    newSize = GetHigher3mod4Prime(newSize);

    if (newSize > map->cap) {
        void *tmp = calloc(newSize, map->usize);
        if (tmp == NULL) {
            return -1;
        }

       

        printf("2 NewSize: %dn", newSize);
        return 0;
    }

        printf("3 NewSize: %s\n");



    return -2;
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

        void **entry = &map->entries[table_index];

        if (NULL == *entry) {
            return NULL;
        } else {
            if (map->compare_cb(unit, *entry, NULL) == true) {
                return *entry;
            }
        }

    }

    return NULL;
}

int _map_set(uint64_t hash, void **entries, size_t cap, void *unit, bool (*compare_cb)(const void *, const void *, void *udata), size_t *count) {
    for (size_t i = 0; i < cap; i++) {
        size_t table_index = hash_get_index(hash, i, cap);

        void **entry = &entries[table_index];

        if (NULL == *entry) {
            *entry = unit;
            *count++;
            return 0;
        } else {
            /** comparison between found entry and value */
            if (compare_cb(unit, *entry, NULL) == true) {
                *entry = unit;
                *count++;
                return 0;
            }
        }
    }

    return -1;
}

int map_set(map_t *map, void *unit)
{
    uint64_t hash = map->hash_cb(unit, NULL);
    int ret = _map_set(hash, map->entries, map->cap, unit, map->compare_cb, &map->count) + 1;
    if (ret == -1) {
        printf("got -1\n");
        return -1;
    }

    printf("got %d\n", ret);
    map->count = (size_t) ret;

    return 0;

    // for (size_t i = 0; i < map->cap; i++) {
    //     size_t table_index = hash_get_index(hash, i, map->cap);

    //     void **entry = &map->entries[table_index];

    //     if (NULL == *entry) {
    //         *entry = unit;
    //         map->count++;
    //         return 0;
    //     } else {
    //         /** comparison between found entry and value */
    //         if (map->compare_cb(unit, *entry, NULL) == true) {
    //             *entry = unit;
    //             map->count++;
    //             return 0;
    //         }
    //     }
    // }
}

void *map_remove(map_t *map, void *unit)
{
    uint64_t hash = map->hash_cb(unit, NULL);
    void *removed_unit = NULL;

    for (size_t i = 0; i < map->cap; i++) {
        size_t table_index = hash_get_index(hash, i, map->cap);

        void **entry = &map->entries[table_index];

        if (NULL == *entry) {
            *entry = unit;
            return NULL;
        } else {
            /** comparison between found entry and value */
            if (map->compare_cb(unit, *entry, NULL) == true) {
                removed_unit = *entry;
                *entry = NULL;
                map->count--;
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
            iter_cb(map->entries[i], NULL);
    }
}

int map_count(map_t *map)
{
    return map->count;
}
