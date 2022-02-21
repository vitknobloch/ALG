#ifndef __LOADER_H__
#define __LOADER_H__

/**
 * @brief Struct to hold information about graphs structure, best solution 
 * and graph search assisting data
 */
typedef struct
{
    int nodec;        //count of all nodes
    int redc;         //count of red nodes
    char *visited;    //for every node char on it's index - true means visited false not visited
    int *neighbours;  //for every node 3 integers with neighbour index (-1 means no neighbour)
    double best_diff; //difference from half of red nodes in current best solution
    int best_l;       //lower node index of best solution
    int best_h;       //higher node index of best solution
} graph_t;

/**
 * @brief Allocates memory for graph and loads it from stdin.
 * Graph allways has to be free'd using free_graph()
 * 
 * @return graph_t* newly loaded graph
 */
graph_t *load_graph();

/**
 * @brief Free's all the allocated resources for graph
 * 
 * @param graph graph to clear (pointer to pointer)
 */
void free_graph(graph_t **graph);

#endif