#include <stdlib.h>
#include <stdio.h>
#include "loader.h"

/**
 * @brief Loads surface data from stdin into new char array and fills rows and cols count.
 * 
 * @param rows int to store number of loaded rows
 * @param cols int to store number of loaded columns
 * @return char* pointer to dynamically allocated surface data or NULL on error
 */
char *load_surface_input(int *rows, int *cols);

/*Allocates memory of given size and return pointer*/
void *allocate_memory(int size);
/*Deallocates memory at given pointer*/
void deallocate_memory(void **ptr);

surface_t load_surface()
{
    surface_t surf = {.rows = -1, .cols = -1, .map = NULL, .col_list = NULL};

    surf.map = load_surface_input(&surf.rows, &surf.cols);

    surf.col_list = allocate_memory(sizeof(column_t) * surf.cols);

    for (int c = 0; c < surf.cols; ++c)
    {
        //initialize rows
        surf.col_list[c].row_uncertain = -1;
        surf.col_list[c].row_unusable = -1;
        const int cols_right = surf.cols - 2 - c;
        if (cols_right > 0)
        {
            //alocate column reachability info
            surf.col_list[c].cols_cost_zero = allocate_memory(sizeof(int) * (surf.cols - 2 - c));
            surf.col_list[c].cols_cost_one = allocate_memory(sizeof(int) * (surf.cols - 2 - c));
            //initialize column reachability info
            for (int i = 0; i < surf.cols - 2 - c; ++i)
            {
                surf.col_list[c].cols_cost_zero[i] = -1;
                surf.col_list[c].cols_cost_one[i] = -1;
            }
        }
        else
        {
            surf.col_list[c].cols_cost_zero = NULL;
            surf.col_list[c].cols_cost_one = NULL;
        }
    }

    return surf;
}

char *load_surface_input(int *rows, int *cols)
{

    if (scanf("%d %d", rows, cols) != 2)
    {
        fprintf(stderr, "Error loading row and column count.\n");
        return NULL;
    }

    char *surface = (char *)allocate_memory(*rows * *cols);

    int c;
    char *surface_writer = surface;
    while ((c = getchar()) != EOF)
    {
        if (c == '\r' || c == '\n')
            continue;

        *surface_writer++ = (char)c;
        for (int i = 1; i < *cols; i++)
        {
            c = getchar();
            *surface_writer++ = (char)c;
        }
    }

    return surface;
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

void free_surface(surface_t *surface)
{
    for (int i = 0; i < surface->cols; ++i)
    {
        deallocate_memory((void **)&surface->col_list[i].cols_cost_one);
        deallocate_memory((void **)&surface->col_list[i].cols_cost_zero);
    }

    deallocate_memory((void **)&surface->col_list);
    deallocate_memory((void **)&surface->map);
}