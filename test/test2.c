#include <map.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "timing.h"

typedef struct function_info {
    void *func;
    const char *str;
}function_info_t;

bool fi_compare(const void *a, const void *b, void *udata)
{
    const function_info_t *fia = a;
    const function_info_t *fib = b;

    if (strcmp(fia->str, fib->str) == 0) {
        return true;
    }

    return false;
}

bool fi_iter(const void *item, void *udata)
{
    const function_info_t *fi = item;
    printf("Function: %p - %s", fi->func, fi->str);
}

uint64_t fi_hash(const void *item)
{
    if (item == NULL) {
        return 0;
    }

    const function_info_t *fi = item;
    uint64_t hash = fnv1a_hash(NULL, fi->func, sizeof *fi->func);

    return hash;
}

void test_function(void)
{

}

void fi_test()
{

    map_t *map = map_create(sizeof(function_info_t), fi_compare, fi_hash);
    if (map == NULL) {
        return;
    }

    map_set(map, &(function_info_t){.func = test_function, .str = "test_function()"});

    function_info_t *fi =  map_get(map, &(function_info_t){.func = test_function, .str = "test_function()"});



    map_foreach(map, fi_iter, NULL);

    map_destroy(map);
}

int main(void)
{
    fi_test();
}