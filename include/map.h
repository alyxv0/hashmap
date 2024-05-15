#ifndef HASHMAP_MAP_H
#define HASHMAP_MAP_H

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#include <pthread.h>

typedef struct bucket {
    pthread_mutex_t lock;
    void *entry;
} bucket_t;

/**
 * @brief Simple map structure
 * 
 */
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
    bucket_t **buckets;

}  map_t;

/**
 * @brief FNV-1A Hash Function
 * 
 * @param hash pointer to possible previous hash (can be NULL)
 * @param mem memory to hash
 * @param len memory length to hash
 * @return uint64_t 
 */
uint64_t fnv1a_hash(uint64_t *hash, const void *mem, size_t len);

/**
 * @brief FNV-1 Hash Function
 * 
 * @param hash pointer to possible previous hash (can be NULL)
 * @param mem memory to hash
 * @param len memory length to hash
 * @return uint64_t 
 */
uint64_t fnv1_hash(uint64_t *hash, const void *mem, size_t len);

/**
 * @brief Creates a hashmap of sized type
 * 
 * @param unitSize size of object
 * @param compare_cb callback for comparing an object
 * @param hash_cb callback for hashing an object
 * @return map_t* 
 */
map_t *map_create(size_t unitSize, bool (*compare_cb)(const void *, const void *, void *),
                  uint64_t (*hash_cb)(const void *));
/**
 * @brief Destroy map
 * 
 * @param map 
 */
void map_destroy(map_t *map);

/**
 * @brief Returns object from given unit to compare to
 * 
 * @param map map object
 * @param unit unit pointer
 * @return void* object or NULL
 */
void *map_get(map_t *map, void *unit);

/**
 * @brief Adds an object to hashmap
 * 
 * @param map map object
 * @param unit unit pointer
 * @return void* 
 */
int map_set(map_t *map, void *unit);

/**
 * @brief Removes an object
 * 
 * @param map 
 * @param unit 
 * @return void* 
 */
void *map_remove(map_t *map, void *unit);


/**
 * @brief For-Each iteration function
 * 
 * 
 * @param map map object
 * @param iter_cb iteration callback
 * @param udata user data (usable in iter_cb)
 */
bool map_foreach(map_t *map, bool (*iter_cb)(const void *, void *), void *udata);

/**
 * @brief Returns number of objects container
 * 
 * @param map map object
 * @return size_t 
 */
size_t map_count(map_t *map);

/**
 * @brief Returns capacity of map
 * 
 * @param map 
 * @return size_t 
 */
size_t map_cap(map_t *map);

#endif