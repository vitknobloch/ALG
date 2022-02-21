#include "loader.h"

#include <stdio.h>

/**
 * @brief Gets the index neighbour_index-th neighbour of the node
 * 
 * @param node node whose neighbour to find
 * @param neighbour_index index of the neighbour among nodes neighbours
 * @param g graph in which to search
 * @return int index of the neighbour node in graph
 */
int get_neighbour(int node, int neighbour_index, graph_t *g);

/**
 * @brief Calculates the number of red nodes in subgraph with root node
 * recursively searches the graph/subgraph
 * 
 * @param node root node of the subgraph
 * @param g graph in which to search
 * @return int number of red nodes in given subgraph
 */
int red_subgraph(int node, graph_t *g);

/**
 * @brief Finds out wheter new solution is better than current best and set it to current best if so
 * 
 * @param n1 node 1 of new solution
 * @param n2 node 2 of new solution
 * @param red number of red nodes in n2's subgraph
 * @param g graph in which to search/set best
 */
void test_new_best(int n1, int n2, int red, graph_t *g);

int main(int argc, char **argv)
{
    graph_t *graph = load_graph();
    if (!graph)
        return 100;

    red_subgraph(0, graph);
    //shift node indexes back so that they match input
    printf("%d %d\r\n", graph->best_l + 1, graph->best_h + 1);

    free_graph(&graph);
    return 0;
}

int get_neighbour(int node, int neighbour_index, graph_t *g)
{
    return g->neighbours[node * 3 + neighbour_index];
}

int red_subgraph(int node, graph_t *g)
{
    //set node as visited and calculate if it is red
    g->visited[node] = 1;
    int red = 0;
    if (node < g->redc)
        ++red;

    //visit all unvisited neighbours and get their subgraph red counts
    for (int i = 0; i < 3; ++i)
    {
        int n = get_neighbour(node, i, g);
        if (n == -1)
            break;
        if (g->visited[n])
            continue;

        int red_n = red_subgraph(n, g);
        //test for new best
        test_new_best(node, n, red_n, g);
        //add neighbours subgraph count to this nodes subgraph count
        red += red_n;
    }

    return red;
}

void test_new_best(int n1, int n2, int red, graph_t *g)
{
    //calculate difference
    double diff = red - (g->redc / 2.0);
    if (diff < 0)
        diff = -diff;

    //if the diff is larger - no new best
    if (diff > g->best_diff)
        return;

    //order nodes
    if (n2 < n1)
    {
        int n = n1;
        n1 = n2;
        n2 = n;
    }

    //if the diff is lower - set new best
    if (diff < g->best_diff)
    {
        g->best_l = n1;
        g->best_h = n2;
        g->best_diff = diff;
        return;
    }

    //if the diff is the same - test for lower node numbers
    if (n1 < g->best_l || (n1 == g->best_l && n2 < g->best_h))
    {
        g->best_l = n1;
        g->best_h = n2;
        g->best_diff = diff;
    }
}
