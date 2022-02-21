#include "loader.h"
#include "my_malloc.h"
#include <stdio.h>

void fill_nodes(dag_t *g);
void fill_edges(dag_t *g, load_t *l);

dag_t *load_dag()
{
    load_t load;
    if (scanf(" %d %d", &load.cinema_c, &load.mov_len) != 2)
        return NULL;

    // Load transport lenght matrix
    load.paths = allocate_memory(sizeof(int) * load.cinema_c * load.cinema_c);
    for (int r = 0; r < load.cinema_c; ++r)
    {
        for (int c = 0; c < load.cinema_c; ++c)
        {
            if (scanf(" %d", &load.paths[r * load.cinema_c + c]) != 1)
                return NULL;
        }
    }

    // Load cinemas and play times in them
    dag_t *g = allocate_memory(sizeof(dag_t));
    g->node_c = 0;
    g->cinema_c = load.cinema_c;
    g->cinemas = allocate_memory(sizeof(cinema_t) * load.cinema_c);
    for (int i = 0; i < load.cinema_c; ++i)
    {
        if (scanf(" %d", &g->cinemas[i].playcount) != 1)
            return NULL;
        g->node_c += g->cinemas[i].playcount;
        g->cinemas[i].plays = allocate_memory(sizeof(int) * g->cinemas[i].playcount);
        for (int p = 0; p < g->cinemas[i].playcount; ++p)
        {
            if (scanf(" %d", &g->cinemas[i].plays[p]) != 1)
                return NULL;
        }
    }

    g->edge_c = 0;
    g->edge_capacity = g->node_c * g->cinema_c;
    g->nodes = allocate_memory(sizeof(node_t) * g->node_c);
    g->edges = allocate_memory(sizeof(edge_t) * g->edge_capacity);

    fill_nodes(g);
    fill_edges(g, &load);
    deallocate_memory((void **)&load.paths);
    return g;
}

void free_dag(dag_t *g)
{
    for (int i = 0; i < g->node_c; ++i)
    {
        deallocate_memory((void **)&g->nodes[i].edges);
    }
    for (int i = 0; i < g->cinema_c; ++i)
    {
        deallocate_memory((void **)&g->cinemas[i].plays);
    }
    deallocate_memory((void **)&g->nodes);
    deallocate_memory((void **)&g->edges);
    deallocate_memory((void **)&g->cinemas);
    deallocate_memory((void **)&g);
}

/* Creates nodes in topological order from playtimes stroded in cinemas
    and fills the cinemas with indexes of the corresponding plays as nodes in graph */
void fill_nodes(dag_t *g)
{
    int *idxs = allocate_memory(sizeof(int) * g->cinema_c);
    for (int i = 0; i < g->cinema_c; ++i)
        idxs[i] = 0;

    for (int n = 0; n < g->node_c; ++n)
    {
        int min_cinema = -1;
        // find cinema with lowest yet uninserted playtime
        for (int c = 0; c < g->cinema_c; ++c)
        {
            if (min_cinema == -1 && idxs[c] < g->cinemas[c].playcount)
                min_cinema = c;
            if (idxs[c] < g->cinemas[c].playcount &&
                g->cinemas[c].plays[idxs[c]] < g->cinemas[min_cinema].plays[idxs[min_cinema]])
                min_cinema = c;
        }
        // insert the minimal playtime as node
        g->nodes[n].edge_c = 0;
        g->nodes[n].edges = allocate_memory(sizeof(int) * g->cinema_c);
        g->nodes[n].parent = -1;
        g->nodes[n].prev_count = 0;
        g->nodes[n].time_spent = 0;
        g->nodes[n].cinema = min_cinema;
        g->nodes[n].time_start = g->cinemas[min_cinema].plays[idxs[min_cinema]];
        g->cinemas[min_cinema].plays[idxs[min_cinema]] = n;
        // update minimal uninserted playtime index for the playtime's cinema
        idxs[min_cinema]++;
    }

    deallocate_memory((void **)&idxs);
}

void fill_edges(dag_t *g, load_t *l)
{
    /* For every cinema remember index of first play in all other cinemas that
     can be reached in time*/
    int *idxs = allocate_memory(sizeof(int) * g->cinema_c * g->cinema_c);
    int e = 0;
    for (int i = 0; i < g->cinema_c * g->cinema_c; ++i)
        idxs[i] = 0;

    for (int n = 0; n < g->node_c; ++n)
    {
        node_t *node = get_node(g, n);
        for (int c = 0; c < g->cinema_c; ++c)
        {
            // skip if can get to no play in the cinema
            if (idxs[node->cinema * g->cinema_c + c] >= g->cinemas[c].playcount)
                continue;
            // minimal time for play start so that the play can be reached in time in that cinema
            int min_time = node->time_start + l->mov_len + l->paths[node->cinema * g->cinema_c + c];
            // iterate through the cinemas plays until reachable play is found
            int node_idx = g->cinemas[c].plays[idxs[node->cinema * g->cinema_c + c]];
            int time_start = get_node(g, node_idx)->time_start;
            while (idxs[node->cinema * g->cinema_c + c] < g->cinemas[c].playcount && min_time > time_start)
            {
                idxs[node->cinema * g->cinema_c + c]++;
                if (idxs[node->cinema * g->cinema_c + c] >= g->cinemas[c].playcount)
                    break;
                node_idx = g->cinemas[c].plays[idxs[node->cinema * g->cinema_c + c]];
                time_start = get_node(g, node_idx)->time_start;
            }

            // insert edge if reachable play exists
            if (idxs[node->cinema * g->cinema_c + c] < g->cinemas[c].playcount)
            {
                g->edges[e].begin = n;
                g->edges[e].end = node_idx;
                g->edges[e].lenght = l->paths[node->cinema * g->cinema_c + c];
                node->edges[node->edge_c++] = e;
                e++;
            }
        }
    }

    deallocate_memory((void **)&idxs);
}
