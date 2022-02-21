#include "iter.h"
#include "my_malloc.h"

void place_block_vertical(demand_t *shape, iter_t *iter, map_t *map, int r, int c);
void place_block_horizontal(demand_t *shape, iter_t *iter, map_t *map, int r, int c);

iter_t *copy_iter(iter_t *src, map_t *map)
{
    iter_t *new = allocate_memory(sizeof(iter_t));
    new->solmap = allocate_memory(sizeof(char) * map->cols * map->rows);
    new->demand_left = allocate_memory(sizeof(int) * map->types);
    new->rest = src->rest;

    new->sum = src->sum;
    for (int i = 0; i < map->cols * map->rows; ++i)
    {
        new->solmap[i] = src->solmap[i];
    }

    for (int i = 0; i < map->types; ++i)
    {
        new->demand_left[i] = src->demand_left[i];
    }

    return new;
}

void free_iter(iter_t *iter)
{
    deallocate_memory((void **)&iter->solmap);
    deallocate_memory((void **)&iter->demand_left);
    deallocate_memory((void **)&iter);
}

void place(int demand_index, iter_t *iter, map_t *map, int r, int c)
{
    iter->demand_left[demand_index] -= 1;
    demand_t *shape = map->demand + demand_index;
    for (int ri = 0; ri < shape->rowspan; ++ri)
    {
        for (int ci = 0; ci < shape->colspan; ++ci)
        {
            iter->solmap[index(map, r + ri, c + ci)] = 1;
            iter->sum += map->map[index(map, r + ri, c + ci)];
            iter->rest -= map->map[index(map, r + ri, c + ci)];
        }
    }

    place_block_horizontal(shape, iter, map, r, c);
    place_block_vertical(shape, iter, map, r, c);
}

void place_block_horizontal(demand_t *shape, iter_t *iter, map_t *map, int r, int c)
{
    if (r > 0)
    {
        for (int i = -1; i < shape->colspan + 1; ++i)
        {
            if (c + i >= 0 && c + i < map->cols)
            {
                if (!tile_state(map, iter, r - 1, c + i))
                {
                    iter->solmap[index(map, r - 1, c + i)] = 2;
                    iter->rest -= map->map[index(map, r - 1, c + i)];
                }
            }
        }
    }

    if (r + shape->rowspan < map->rows)
    {
        for (int i = -1; i < shape->colspan + 1; ++i)
        {
            if (c + i >= 0 && c + i < map->cols)
            {
                if (!tile_state(map, iter, r + shape->rowspan, c + i))
                {
                    iter->solmap[index(map, r + shape->rowspan, c + i)] = 2;
                    iter->rest -= map->map[index(map, r + shape->rowspan, c + i)];
                }
            }
        }
    }
}

void place_block_vertical(demand_t *shape, iter_t *iter, map_t *map, int r, int c)
{
    if (c > 0)
    {
        for (int i = 0; i < shape->rowspan; ++i)
        {
            if (r + i < map->rows)
            {
                if (!tile_state(map, iter, r + i, c - 1))
                {
                    iter->solmap[index(map, r + i, c - 1)] = 2;
                    iter->rest -= map->map[index(map, r + i, c - 1)];
                }
            }
        }
    }

    if (c + shape->colspan < map->cols)
    {
        for (int i = 0; i < shape->rowspan; ++i)
        {
            if (r + i < map->rows)
            {
                if (!tile_state(map, iter, r + i, c + shape->colspan))
                {
                    iter->solmap[index(map, r + i, c + shape->colspan)] = 2;
                    iter->rest -= map->map[index(map, r + i, c + shape->colspan)];
                }
            }
        }
    }
}

int blocked(demand_t *shape, iter_t *iter, map_t *map, int r, int c)
{
    if (r + shape->rowspan > map->rows || c + shape->colspan > map->cols)
        return 1;

    for (int ri = 0; ri < shape->rowspan; ++ri)
    {
        for (int ci = 0; ci < shape->colspan; ++ci)
        {
            if (tile_state(map, iter, r + ri, c + ci))
                return 1;
        }
    }
    return 0;
}
