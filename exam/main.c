#include "dag.h"
#include "loader.h"
#include <stdio.h>

void print_graph(dag_t *g);

int main(int argc, char *argv[])
{
    dag_t *g = load_dag();
    // print_graph(g);
    int last = longest_path(g);
    node_t *last_node = get_node(g, last);
    printf("%d %d\r\n", last_node->prev_count + 1, last_node->time_spent);
    free_dag(g);
    return 0;
}

void print_graph(dag_t *g)
{
    for (int n = 0; n < g->node_c; ++n)
    {
        node_t *node = get_node(g, n);
        printf("%d - %d %d\n", n, node->time_start, node->cinema);
        for (int e = 0; e < node->edge_c; ++e)
        {
            edge_t *edge = get_edge(g, node->edges[e]);
            printf(" : %d %d", edge->end, edge->lenght);
        }
        printf("\n");
    }
}
