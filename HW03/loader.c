#include "loader.h"
#include <stdio.h>
#include <stdlib.h>

/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size);
/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr);
/*Sets new neighbour of the node to be the neighbour*/
void set_neighbour(int node, int neighbour, graph_t *g);

graph_t *load_graph()
{
    graph_t *graph = allocate_memory(sizeof(graph_t)); //struct memory

    if (scanf("%d %d", &graph->nodec, &graph->redc) != 2) //load graph size
    {
        deallocate_memory((void **)&graph);
        return NULL;
    }

    //allocate and init nodes neighbour lists memory
    graph->neighbours = allocate_memory(sizeof(int) * 3 * graph->nodec);
    for (int i = 0; i < graph->nodec * 3; ++i)
    {
        graph->neighbours[i] = -1;
    }

    //allocate and init nodes visited indicator
    graph->visited = allocate_memory(graph->nodec);
    for (int i = 0; i < graph->nodec; ++i)
    {
        graph->visited[i] = 0;
    }

    //load edges from stdin and fill neighbour lists
    for (int i = 0; i < graph->nodec - 1; ++i)
    {
        int n1, n2;
        if (scanf("%d %d", &n1, &n2) != 2)
        {
            free_graph(&graph);
            return NULL;
        }
        set_neighbour(n1, n2, graph);
        set_neighbour(n2, n1, graph);
    }

    //init best solution
    graph->best_diff = graph->redc;
    graph->best_l = graph->nodec;
    graph->best_h = graph->nodec;

    return graph;
}

void free_graph(graph_t **graph)
{
    deallocate_memory((void **)&(*graph)->neighbours);
    deallocate_memory((void **)&(*graph)->visited);
    deallocate_memory((void **)graph);
}

void set_neighbour(int node, int neighbour, graph_t *g)
{
    //shift node indexes by one (so that they start with zero)
    --node;
    --neighbour;
    int index = node * 3;
    while (g->neighbours[index] != -1)
    {
        ++index;
    }
    g->neighbours[index] = neighbour;
}

/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
    return ptr;
}

/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr)
{
    if (*ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL;
    }
}