#include <stdio.h>
#include <stdlib.h>

#include "loader.h"
#include "solver.h"

int main(int argc, char **argv)
{
    surface_t surface = load_surface();

    printf("%d\n", solve(&surface));

    free_surface(&surface);
}
