#include <map.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>


typedef struct user {
    char *name;
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
    const user_t *user = item;
    uint64_t hash = fnv1a_hash(NULL, user->name, strlen(user->name));
    return hash;
}

int main(void)
{

    map_t *map = map_create(sizeof(user_t), user_compare, user_hash);
    if (map == NULL) {
        fprintf(stderr, "error: (to implement)");
        abort();
    }

    

    map_set(map, &(user_t) {.name = "Flo1", .age=25});
    map_set(map, &(user_t) {.name = "Flo2", .age=25});
    map_set(map, &(user_t) {.name = "Flo3", .age=25});
    map_set(map, &(user_t) {.name = "Flo4", .age=25});
    map_set(map, &(user_t) {.name = "Flo5", .age=25});
    map_set(map, &(user_t) {.name = "Flo6", .age=25});
    map_set(map, &(user_t) {.name = "Flo7", .age=25});

    map_realloc(map);


    printf("Count: %d\n", map_count(map));
    map_scan(map, user_iter);

    user_t *user = map_get(map, &(struct user){.name = "Flo1"});

    map_remove(map, &(struct user){.name = "Flo1"});

    printf("After remove:\n");
    map_scan(map, user_iter);


    map_destroy(map);

    return 0;
}