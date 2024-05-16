#include "include/utils.h"

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>


#define DEFAULT_HASH_CAPACITY 7
#define DEFAULT_LOAD_FACTOR 0.6

typedef struct bucket {
    pthread_mutex_t lock;
    void *entry;
} bucket_t;

typedef struct map
{
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
    bucket_t *buckets;

}  map_t;

int _object_insert(map_t *map, bucket_t *entries, size_t cap, void *unit);

void map_rehash(map_t *map, bucket_t *entries, size_t new_cap)
{
    for (size_t i = 0; i < map->cap; i++) {

        int err = 0;
        if (map->buckets[i].entry != NULL) {
            if ((err = _object_insert(map, entries, new_cap, map->buckets[i].entry)) == -1) {
                fprintf(stderr, "warning: failed to insert object\n");
            }
            entries[err].lock = map->buckets[i].lock;
        }
    }
}

int map_realloc(map_t *map)
{
    size_t newSize = 0;
    if (map->count >= (size_t)((double)map->cap * DEFAULT_LOAD_FACTOR)) {
        newSize = (size_t)(map->cap * (double)(1 + DEFAULT_LOAD_FACTOR));
    }

    if (newSize == 0) {
        return 0;
    }

    newSize = GetHigher3mod4Prime(newSize);

    if (newSize < DEFAULT_HASH_CAPACITY) {
        newSize = DEFAULT_HASH_CAPACITY;
    }

    bucket_t *tmp = calloc(newSize, sizeof *map->buckets);
    if (tmp == NULL) {
        return -1;
    }

        for (size_t i = 0; i < map->cap; i++) {
        tmp[i].lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    }


    // rehash, overwrite with new map->cap, free old entries, overwrite map entries pointer
    map_rehash(map, tmp, newSize);
    map->cap = newSize;
    free(map->buckets);
    map->buckets = tmp;

    return 0;
}

map_t *map_create(size_t unitSize, bool (*compare_cb)(const void *, const void *, void *udata),
                  uint64_t (*hash_cb)(const void *, void *udata))
{
    map_t *map = calloc(1, sizeof *map);
    if (NULL == map) {
        return NULL;
    }

    map->buckets = calloc(DEFAULT_HASH_CAPACITY, sizeof *map->buckets);
    if (NULL == map->buckets) {
        free(map);
        return NULL;
    }

    for (size_t i = 0; i < map->cap; i++) {
        map->buckets[i].lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    }


    map->cap = DEFAULT_HASH_CAPACITY;
    map->usize = unitSize;
    map->compare_cb = compare_cb;
    map->hash_cb = hash_cb;

    return map;
}

void map_destroy(map_t *map)
{
    free(map->buckets);
    free(map);
}

void* map_get(map_t *map, void *unit)
{
    uint64_t hash = map->hash_cb(unit, NULL);

    for (size_t i = 0; i < map->cap; i++) {
        size_t table_index = hash_get_index(hash, i, map->cap);
        
        printf("map_get: lock\n");
        pthread_mutex_lock(&map->buckets[table_index].lock);

        // printf("%zu\n", table_index);

        if (NULL == map->buckets[table_index].entry) {
            continue;
        } else {
            if (map->compare_cb(unit, map->buckets[table_index].entry, NULL) == true) {
                printf("map_get: found unlock\n");
                pthread_mutex_unlock(&map->buckets[table_index].lock);
                return map->buckets[table_index].entry;
            }
        }
        printf("map_get: unlock\n");
        pthread_mutex_unlock(&map->buckets[table_index].lock);
    }

    return NULL;
}

int _object_insert(map_t *map, bucket_t *entries, size_t cap, void *unit)
{
    size_t table_index = 0;
    uint64_t hash = map->hash_cb(unit, NULL);

    for (size_t i = 0; i < cap; i++) {

        table_index = hash_get_index(hash, i, cap);

        // printf("%p\n", entries[table_index]);
        
        pthread_mutex_lock(&map->buckets[table_index].lock);
        // printf("%zu: _object_insert: lock\n", i);

        if (entries[table_index].entry == NULL) {
            entries[table_index].entry = unit;
            // printf("%zu: _object_insert: unlock\n", i);
            // pthread_mutex_unlock(&map->buckets[table_index].lock);
            return table_index;
        }

        if (map->compare_cb(entries[table_index].entry, unit, NULL) == true) {
            entries[table_index].entry = unit;
            // printf("%zu: _object_insert: unlock\n", i);
            // pthread_mutex_unlock(&map->buckets[table_index].lock);
            return table_index;
        }
        // pthread_mutex_unlock(&map->buckets[table_index].lock);
    }

    return -1;
}

int map_set(map_t *map, void *unit)
{
    uint64_t hash = map->hash_cb(unit, NULL);

    size_t index = -1;

    if ((index = _object_insert(map, map->buckets, map->cap, unit)) == -1) {
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

    if (unit == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < map->cap; i++) {
        size_t table_index = hash_get_index(hash, i, map->cap);

        if (NULL == map->buckets[table_index].entry)
        {
            continue;
        }
        else
        {
            /** comparison between found entry and value */
            if (map->compare_cb(unit, map->buckets[table_index].entry, NULL) == true) {
                removed_unit = map->buckets[table_index].entry;
                map->buckets[table_index].entry = NULL;
                map->count--;
                map_realloc(map);
                // printf("Found and remove!\n");
                return removed_unit;
            }
        }
    }
    return NULL;
}

bool map_foreach(map_t *map, bool (*iter_cb)(const void *, const void *), void *udata)
{
    bool result = false;

    for (size_t i = 0; i < map->cap; i++) {
        if (map->buckets[i].entry != NULL) {
            // pthread_mutex_lock(&map->buckets[i].lock);
            result = iter_cb(map->buckets[i].entry, udata);
            // pthread_mutex_unlock(&map->buckets[i].lock);
            if (result == false)
                break;
        }
    }

    return result;
}

int map_count(map_t *map)
{
    return map->count;
}

size_t map_cap(map_t *map)
{
    return map->cap;
}