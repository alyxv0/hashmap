#include <map.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "timing.h"

#include <limits.h>

typedef struct function_info {
    void *func;
    const char *str;
}function_info_t;

bool fi_compare(const void *a, const void *b, void *udata)
{
    const function_info_t *fia = a;
    const function_info_t *fib = b;

    if (fia->func == fib->func) {
        return true;
    }

    return false;
}

bool fi_iter(const void *item, void *udata)
{
    const function_info_t *fi = item;
    printf("Object: %p, Function: %p - %s\n", fi, fi == NULL ? NULL : fi->func, fi == NULL ? NULL : fi->str);
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

    timer_start();
    map_t *map = map_create(sizeof(function_info_t), fi_compare, fi_hash);
    timer_stop();
    if (map == NULL) {
        return;
    }
    printf("Map-Create: %s\n", timer_print());

    timer_start();
    map_set(map, &(function_info_t){.func = test_function, .str = "test_function()"});
    map_set(map, &(function_info_t){.func = test_function, .str = "test_function()"});
    map_set(map, &(function_info_t){.func = test_function, .str = "test_function()"});
    map_set(map, &(function_info_t){.func = test_function, .str = "test_function()"});
    map_set(map, &(function_info_t){.func = test_function, .str = "test_function()"});
    timer_stop();
    printf("Map-Set(5): %s\n", timer_print());

    
    timer_start();
    function_info_t *fi =  map_get(map, &(function_info_t){.func = test_function, .str = "test_function()"});
    timer_stop();
    printf("Map-Get(1): %s\n", timer_print());


    map_foreach(map, fi_iter, NULL);

    map_destroy(map);

    printf("%s\n", byte_units(UINT64_MAX, TERABYTE));
    printf("%s\n", byte_units(UINT64_MAX, -1));


}

int main(void)
{
    fi_test();
}