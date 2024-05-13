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
    if (strcmp(ua->name, ub->name) == 0) {
        return true;
    }
    return false;
}

bool user_iter(const void *item, void *udata)
{
    const user_t *user = item;
    printf("%s (age: %d)\n", user->name, user->age);
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
    hash = fnv1a_hash(&hash, &user->age, sizeof(user->age));
    return hash;
}

int range(int min, int max)
{
    return (rand() % (max + 1 - min) + min);
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
    }
    timer_stop();
    printf("Set Time: %.2fs\n", (double)(timer_time() / CLOCKS_PER_SEC));

    srand(10);
    int r = range(0, user_count);

    timer_start();
    user_t *u = map_get(map, &user[r]);
    timer_stop();
    printf("Get One Time: %.2lfms\n", timer_time() / 1000);

    timer_start();
    for (size_t i = 0; i < 10; i++)
    {
        r = range(0, user_count);
        u = map_get(map, &user[r]);
        printf("Get for index [%d]User: %s\n", r, u->name);
    }
    timer_stop();
    printf("Get 10 Time: %.2lfms\n", timer_time() / 1000);

    printf("Entries: %zu/%zu\n", map_count(map), map_cap(map));
}

int main(void)
{

    user_test(1024 * 1024 * 40);
    return 0;
}