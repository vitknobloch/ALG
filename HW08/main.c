#include <stdio.h>
#include "totems.h"

void print_table(int *table, int size);

int main(int argc, char *argv[])
{
    tribe_t *tribe = load_tribe();
    fill_diff_table(tribe);
    fill_sum_table(tribe);
    /*printf("Diff table:\n");
    print_table(tribe->diff_table, tribe->village_c);
    printf("Cost table:\n");
    print_table(tribe->cost_table, tribe->village_c);
    printf("Sum table:\n");
    print_table(tribe->sum_table, tribe->village_c);*/
    printf("%d\r\n", get_best_sum(tribe));
    free_tribe(tribe);
    return 0;
}

void print_table(int *table, int size)
{
    for (int r = 0; r < size; ++r)
    {
        for (int c = 0; c < size; ++c)
        {
            if (c < r)
                printf("%3d ", 0);
            else
                printf("%3d ", table[r * size + c]);
        }
        printf("\n\n");
    }
}
