#ifndef __TYPES_H__
#define __TYPES_H__

typedef struct
{
    int row_unusable;    //last row with bad quality or second to last row with uncertain quality
    int row_uncertain;   //last row to have uncertain quality
    int *cols_cost_zero; //for every column to the right number of last row where you get to this column with good tiles only
    int *cols_cost_one;  //for every column to the right number of last row where you get to this column with one uncertain tile
} column_t;

/* Struct containing information about the surface */
typedef struct
{
    int cols;           //number of columns of the surface
    int rows;           //number of rows of the surface
    char *map;          //array with map of surface
    column_t *col_list; //array with column_t struct for every column
} surface_t;

#endif