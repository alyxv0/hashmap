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
    char name[40];
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

    printf("remove user: %s\n", u->name);
    user_t *rem = map_remove(map, (void *)u);
    if (rem == NULL)
        abort();

    return true;
}

void user_test(int user_count)
{

    map_t *map = map_create(sizeof(user_t), user_compare, user_hash);
    if (map == NULL)
    {
        fprintf(stderr, "error: (to implement)");
        abort();
    }
    user_t *user = calloc(user_count, sizeof *user);
    if (user == NULL)
    {
        fprintf(stderr, "error: memory error\n");
        abort();
    }

    timer_start();
    for (size_t i = 0; i < user_count; i++)
    {
        int ret = snprintf(user[i].name, sizeof(user[i].name), "User%zu", i);
        if (ret < 0)
        {
            abort();
        }
        user[i].age = i;

        // printf("i: %zu, name: %s\n", i, user[i].name);
    }
    timer_stop();
    printf("Create Users Time: %.2fs\n", (double)(timer_time() / CLOCKS_PER_SEC));

    timer_start();
    for (size_t i = 0; i < user_count; i++)
    {
        map_set(map, &user[i]);
        printf("name: %s\n", user[i].name);
    }
    timer_stop();
    printf("Set Time: %.2fs\n", (double)(timer_time() / CLOCKS_PER_SEC));

    srand(10);
    int r = range(0, user_count);

    timer_start();
    user_t *u = map_get(map, &user[r]);
    printf("Get for index [%d]User: %s\n", r, u->name);
    timer_stop();
    printf("Get 1 User: %.2lfms\n", timer_time() / 1000);

    timer_start();
    for (size_t i = 0; i < user_count; i++)
    {
        u = map_get(map, &user[i]);
        printf("Get for index [%d]User: %s\n", i, u->name);
    }
    timer_stop();
    printf("Get %d Users: %.2lfms\n", user_count, timer_time() / 1000);

    printf("User0: %p\n", map_get(map, &(user_t){.name = "User0", .age = 0}));
    printf("Entries: %zu/%zu\n", map_count(map), map_cap(map));

    timer_start();
    printf("User0: %p\n", map_get(map, &(user_t){.name = "User0", .age = 0}));
    map_scan(map, use_test_remove_iter, map);
    timer_stop();
    printf("Remove all: %.2lfms\n", timer_time() / 1000);

    printf("Entries: %zu/%zu\n", map_count(map), map_cap(map));

    printf("User0: %p\n", map_get(map, &(user_t){.name = "User0", .age = 0}));
    map_scan(map, use_test_remove_iter, map);

    map_remove(map, &(user_t){.name = "User11", .age = 11});

    map_scan(map, user_iter, NULL);

    printf("Count: %zu\n", map_count(map));
    printf("Cap: %zu\n", map_cap(map));
}

int main(void)
{

    user_test(1024 * 100);
    return 0;
}