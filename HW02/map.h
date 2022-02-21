#ifndef __MAP_H__
#define __MAP_H__

typedef struct
{
    int rowspan;
    int colspan;
    int count;
} demand_t;

typedef struct
{
    int rows;
    int cols;
    unsigned char *map;
    int types;
    demand_t *demand;
    int best_sum;
    int sum;
} map_t;

map_t *load_map();
void free_map(map_t *map);

inline int index(map_t *map, int r, int c)
{
    return r * map->cols + c;
}
inline int get_pos(map_t *map, int r, int c)
{
    return map->map[index(map, r, c)];
}

#endif