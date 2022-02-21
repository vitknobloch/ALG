#include "graph.h"
#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>

void node_add_neighbour(graph_t *g, int node, int neighbour);

graph_t *load_graph()
{
    graph_t *g = allocate_memory(sizeof(graph_t));
    int edgec;
    if (scanf("%d %d %d %d", &g->nodec, &edgec, &g->friendlyc, &g->friendly_feed) != 4)
    {
        exit(100);
    }

    g->nodes = allocate_memory(sizeof(node_t) * g->nodec);
    int default_neighbour_capacity = edgec * 2 / g->nodec + 1;
    for (int i = 0; i < g->nodec; ++i)
    {
        node_t *n = g->nodes + i;
        n->most_fed = -1;
        n->least_packs = __INT_MAX__;
        n->neighbour_capacity = default_neighbour_capacity;
        n->neighbours = allocate_memory(sizeof(int) * n->neighbour_capacity);
        n->neighbour_count = 0;
    }

    for (int i = 0; i < edgec; ++i)
    {
        int n1, n2;
        if (scanf("%d %d", &n1, &n2) != 2)
        {
            exit(101);
        }
        node_add_neighbour(g, n1 - 1, n2 - 1);
        node_add_neighbour(g, n2 - 1, n1 - 1);
    }
    return g;
}

void node_add_neighbour(graph_t *g, int node, int neighbour)
{
    node_t *n = &g->nodes[node];
    if (n->neighbour_count >= n->neighbour_capacity)
    {
        reallocate_memory((void **)&n->neighbours, sizeof(int) * n->neighbour_capacity * 2);
    }
    n->neighbours[n->neighbour_count] = neighbour;
    n->neighbour_count++;
}

void free_graph(graph_t *g)
{
    for (int i = 0; i < g->nodec; ++i)
    {
        deallocate_memory((void **)&g->nodes[i].neighbours);
    }
    deallocate_memory((void **)&g->nodes);
    deallocate_memory((void **)&g);
}