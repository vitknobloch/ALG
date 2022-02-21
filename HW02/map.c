#include "map.h"
#include "my_malloc.h"
#include <stdio.h>

void sort_demand(map_t *map);

map_t *load_map()
{
    map_t *map = allocate_memory(sizeof(map_t));
    map->best_sum = 0;
    map->sum = 0;

    scanf("%d %d", &map->rows, &map->cols);
    map->map = allocate_memory(map->rows * map->cols);
    for (int i = 0; i < map->rows * map->cols; ++i)
    {
        scanf(" %hhu", map->map + i);
        map->sum += map->map[i];
    }

    scanf("%d", &map->types);
    map->demand = allocate_memory(sizeof(demand_t) * map->types);
    for (int i = 0; i < map->types; ++i)
    {
        demand_t *d = map->demand + i;
        scanf(" %d %d %d", &d->rowspan, &d->colspan, &d->count);
    }

    sort_demand(map);

    return map;
}

void sort_demand(map_t *map)
{
    int area[map->types];
    demand_t *sorted = allocate_memory(sizeof(demand_t) * map->types);

    for (int i = 0; i < map->types; ++i)
    {
        area[i] = map->demand[i].rowspan * map->demand[i].colspan;
    }
    for (int i = 0; i < map->types; ++i)
    {
        int best_index = 0;
        for (int j = 0; j < map->types; ++j)
        {
            if (area[j] > area[best_index])
                best_index = j;
        }
        sorted[i] = map->demand[best_index];
        area[best_index] = 0;
    }
    deallocate_memory((void **)&map->demand);
    map->demand = sorted;
}

void free_map(map_t *map)
{
    deallocate_memory((void **)&map->map);
    deallocate_memory((void **)&map->demand);
    deallocate_memory((void **)&map);
}