#include "map.h"
#include "iter.h"
#include "solver.h"

#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"

int main(int argc, char *argv[])
{
    //load map and demand
    map_t *map = load_map();

    printf("%d\n", solve(map));

    //free resources
    free_map(map);

    return 0;
}