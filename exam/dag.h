#ifndef __DAG_H__
#define __DAG_H__

typedef struct cinema
{
    int playcount;
    int *plays;
} cinema_t;

typedef struct node
{
    int cinema;
    int parent;
    int time_start;
    int time_spent;
    int prev_count;
    int edge_c;
    int *edges;
} node_t;

typedef struct edge
{
    int begin;
    int end;
    int lenght;
} edge_t;

typedef struct dag
{
    int node_c;
    node_t *nodes;
    int edge_c;
    int edge_capacity;
    edge_t *edges;
    int cinema_c;
    cinema_t *cinemas;
} dag_t;

int longest_path(dag_t *dag);

inline node_t *get_node(dag_t *dag, int idx)
{
    return &dag->nodes[idx];
}

inline edge_t *get_edge(dag_t *dag, int idx)
{
    return &dag->edges[idx];
}

#endif