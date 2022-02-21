#ifndef __TOTEMS_H__
#define __TOTEMS_H__

typedef struct tribe
{
    int village_c;
    int *villages;
    int totem_price;
    int diff_punish;
    int *diff_table;
    int *count_table;
    int *cost_table;
    int *sum_table;
} tribe_t;

tribe_t *load_tribe();

void free_tribe(tribe_t *tribe);

void fill_diff_table(tribe_t *t);

void fill_sum_table(tribe_t *t);

int get_best_sum(tribe_t *t);

#endif