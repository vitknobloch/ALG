#ifndef __LOADER_H__
#define __LOADER_H__

#include "dag.h"

typedef struct
{
    int cinema_c;
    int *paths;
    int mov_len;
} load_t;

dag_t *load_dag();

void free_dag(dag_t *dag);

#endif
