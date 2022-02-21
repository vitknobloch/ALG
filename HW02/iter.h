#ifndef __ITER_H__
#define __ITER_H__

#include "map.h"

typedef struct
{
    char *solmap;
    int *demand_left;
    int sum;
    int rest;
} iter_t;

iter_t *copy_iter(iter_t *src, map_t *map);
void free_iter(iter_t *iter);
void place(int demand_index, iter_t *iter, map_t *map, int r, int c);
int blocked(demand_t *shape, iter_t *iter, map_t *map, int r, int c);

inline int tile_state(map_t *map, iter_t *iter, int r, int c)
{
    return iter->solmap[r * map->cols + c];
}

#endif