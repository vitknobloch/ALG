#include "solver.h"
#include "my_malloc.h"

#include <stdio.h>

void solve_recursive(iter_t *parent, map_t *map, int index);

int solve(map_t *map)
{
    iter_t *root = allocate_memory(sizeof(iter_t));
    root->solmap = allocate_memory(sizeof(char) * map->cols * map->rows);
    root->demand_left = allocate_memory(sizeof(int) * map->types);

    root->sum = 0;
    root->rest = map->sum;
    for (int i = 0; i < map->cols * map->rows; ++i)
    {
        root->solmap[i] = 0;
    }

    for (int i = 0; i < map->types; ++i)
    {
        root->demand_left[i] = map->demand[i].count;
    }

    solve_recursive(root, map, 0);

    free_iter(root);

    return map->best_sum;
}

void solve_recursive(iter_t *parent, map_t *map, int index)
{
    while (
        index < map->cols * map->rows &&
        tile_state(map, parent, index / map->cols, index % map->cols))
    {
        index++;
    }

    const int r = index / map->cols;
    const int c = index % map->cols;

    if (parent->sum + parent->rest <= map->best_sum)
        return;

    if (parent->sum > map->best_sum)
    {
        map->best_sum = parent->sum;
    }

    for (int i = 0; i < map->types; ++i)
    {
        if (parent->demand_left[i] <= 0)
            continue;
        if (blocked(map->demand + i, parent, map, r, c))
            continue;

        iter_t *child = copy_iter(parent, map);
        place(i, child, map, r, c);
        solve_recursive(child, map, index + map->demand[i].colspan);

        free_iter(child);
    }

    if (index + 1 < map->rows * map->cols)
    {
        iter_t *child = copy_iter(parent, map);
        child->rest -= map->map[index];
        solve_recursive(child, map, index + 1);
        free_iter(child);
    }
}