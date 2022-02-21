#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "queue.h"

typedef struct
{
    int most_fed;
    int least_packs;
    int neighbour_count;
    int neighbour_capacity;
    int *neighbours;
} node_t;

typedef struct
{
    int nodec;
    int friendlyc;
    int friendly_feed;
    node_t *nodes;
} graph_t;

graph_t *load_graph();
void free_graph(graph_t *g);

#endif