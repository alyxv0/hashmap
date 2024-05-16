#include <map.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

#include "timing.h"

typedef struct user {
    char name[60];
    int age;
} user_t;

bool user_compare(const void *a, const void *b, void *udata)
{
    const user_t *ua = a;
    const user_t *ub = b;
    if (strcmp(ua->name, ub->name) == 0)
    {
        return true;
    }

    return false;
}

bool user_iter(const void *item, void *udata)
{
    const user_t *user = item;
    printf("%p: %s (age: %d)\n", user, user->name, user->age);
    return true;
}

uint64_t user_hash(const void *item)
{
    if (item == NULL)
    {
        return 0;
    }

    const user_t *user = item;
    uint64_t hash = fnv1a_hash(NULL, user->name, strlen(user->name));
    return hash;
}

int range(int min, int max)
{
    return (rand() % (max + 1 - min) + min);
}

bool use_test_remove_iter(const void *item, void *udata)
{
    map_t *map = udata;
    const user_t *u = item;

    // printf("remove user: %s\n", u->name);
    user_t *rem = map_remove(map, (void *)u);
    if (rem == NULL)
        abort();

    return true;
}
user_t user = {};

void user_test(int user_count)
{

    map_t *map = map_create(sizeof(user_t), user_compare, user_hash);
    if (map == NULL)
    {
        fprintf(stderr, "error: (to implement)");
        abort();
    }
    printf("Map mem: %s\n", byte_units(user_count * map->usize * 2, -1));

    timer_start();
    for (size_t i = 0; i < user_count; i++) {
        int ret = snprintf(user.name, sizeof(user.name), "User%zu", i);
        if (ret < 0)
        {
            abort();
        }
        user.age = i;
        if (map_set(map, &user) == -1) {
            break;
        }
    }

    timer_stop();
    printf("Set Time: %s\n", timer_print());

    printf("Entries: %zu/%zu\n", map_count(map), map_cap(map));

    srand(10);
    int r = range(0, user_count);

    timer_start();
    int ret = snprintf(user.name, sizeof(user.name), "User%zu", r);
        if (ret < 0)
        {
            abort();
        }
    user.age = r;
    user_t *u = map_get(map, &user);
    timer_stop();
    printf("Get User: %s - %s\n", u->name, timer_print());

    timer_start();
    for (size_t i = 0; i < map_count(map); i++) {
        int ret = snprintf(user.name, sizeof(user.name), "User%zu", i);
        if (ret < 0)
        {
            abort();
        }
        user.age = i;
        u = map_get(map, &user);
        // printf("Get for index [%d]User: %s\n", i, u->name);
    }
    timer_stop();
    printf("Get %d Users: %s\n", user_count, timer_print());

    printf("Entries: %zu/%zu\n", map_count(map), map_cap(map));



    timer_start();
    ret = snprintf(user.name, sizeof(user.name), "User%zu", 0);
        if (ret < 0)
        {
            abort();
        }
        user.age = 0;

    void *tmp = map_get(map, &user);
    timer_stop();
    printf("Get User: %s - %s\n",((user_t*)tmp)->name, timer_print());
    
    timer_start();
    tmp = map_get(map, &(user_t){.name = "User25263", .age = 25263});
    timer_stop();
    printf("Get User: %s - %s\n", ((user_t*)tmp)->name, timer_print());

    printf("map mem: %s\n", byte_units(map->cap * map->usize, -1));


    timer_start();
    map_foreach(map, use_test_remove_iter, map);
    timer_stop();
    printf("Remove all: %s\n", timer_print());
    printf("Entries: %zu/%zu\n", map_count(map), map_cap(map));


    map_remove(map, &(user_t){.name = "User11", .age = 11});

    timer_start();
    // check if everything was removed
    for (size_t i = 0; i < map->cap; i++) {
        if (map->buckets[i].entry != NULL)
        {
            user_iter(map->buckets[i].entry, NULL);
        }
    }
    timer_stop();
    printf("Check all entries if null: %s\n", timer_print());
    printf("map mem: %s\n", byte_units(map->cap * map->usize, -1));

    map_destroy(map);
}



int main(void)
{

    user_test(1024 * 1024);
    return 0;
}