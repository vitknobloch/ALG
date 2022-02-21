#include "dag.h"

int longest_path(dag_t *dag)
{
    int max_plays = 0;
    int best_time_spent = 0;
    int max_idx = 0;
    // iter nodes in topological order
    for (int i = 0; i < dag->node_c; ++i)
    {
        node_t *node = get_node(dag, i);
        // iter through neighbours
        for (int o = 0; o < node->edge_c; ++o)
        {
            edge_t *edge = get_edge(dag, node->edges[o]);
            node_t *neigh = get_node(dag, edge->end);
            // update max cost of neighbour
            if ((neigh->prev_count < node->prev_count + 1) || (neigh->prev_count == node->prev_count + 1 && neigh->time_spent > node->time_spent + edge->lenght))
            {
                neigh->prev_count = node->prev_count + 1;
                neigh->time_spent = node->time_spent + edge->lenght;
                neigh->parent = edge->begin;
                if (neigh->prev_count > max_plays || (neigh->prev_count == max_plays && neigh->time_spent < best_time_spent)) // update max cost
                {
                    max_plays = neigh->prev_count;
                    best_time_spent = neigh->time_spent;
                    max_idx = edge->end;
                }
            }
        }
    }
    return max_idx;
}
