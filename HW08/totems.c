#include "totems.h"
#include "my_malloc.h"
#include <stdio.h>

inline int get_idx(int r, int c, int village_count);
void fill_diff_spot(tribe_t *t, int r, int c);
void fill_sum_spot(tribe_t *t, int r, int c);

void fill_diff_table(tribe_t *t)
{
    for (int diff = 1; diff < t->village_c; ++diff)
    {
        for (int c = diff; c < t->village_c; ++c)
        {
            int r = c - diff;
            fill_diff_spot(t, r, c);
        }
    }
}

void fill_diff_spot(tribe_t *t, int r, int c)
{
    int best_cost = __INT_MAX__;
    for (int i = 0; i < c - r; ++i)
    {
        int count1 = t->count_table[get_idx(r, r + i, t->village_c)];
        int count2 = t->count_table[get_idx(r + i + 1, c, t->village_c)];
        int count_diff = count1 - count2;
        if (count_diff < 0)
            count_diff = -count_diff;
        int cost1 = t->diff_table[get_idx(r, r + i, t->village_c)];
        int cost2 = t->diff_table[get_idx(r + i + 1, c, t->village_c)];
        int cost_sum = cost1 + cost2;
        int total_cost = cost_sum + count_diff;
        if (total_cost < best_cost)
            best_cost = total_cost;
    }

    t->count_table[get_idx(r, c, t->village_c)] =
        t->count_table[get_idx(r, c - 1, t->village_c)] + t->villages[c];
    t->diff_table[get_idx(r, c, t->village_c)] = best_cost;
    t->cost_table[get_idx(r, c, t->village_c)] = (c - r) * t->totem_price - best_cost * t->diff_punish;
}

void fill_sum_spot(tribe_t *t, int r, int c)
{
    int best_sum = t->cost_table[get_idx(r, c, t->village_c)];
    for (int i = 0; i < c - r; ++i)
    {
        int sum1 = t->sum_table[get_idx(r, r + i, t->village_c)];
        int sum2 = t->sum_table[get_idx(r + i + 1, c, t->village_c)];
        int sum = sum1 + sum2;
        if (sum > best_sum)
            best_sum = sum;
    }

    t->sum_table[get_idx(r, c, t->village_c)] = best_sum;
}

void fill_sum_table(tribe_t *t)
{
    for (int diff = 1; diff < t->village_c; ++diff)
    {
        for (int c = diff; c < t->village_c; ++c)
        {
            int r = c - diff;
            fill_sum_spot(t, r, c);
        }
    }
}

tribe_t *load_tribe()
{
    tribe_t *tribe = allocate_memory(sizeof(tribe_t));
    if ((scanf("%d %d %d", &tribe->village_c, &tribe->totem_price, &tribe->diff_punish)) != 3)
        return NULL;

    tribe->villages = allocate_memory(sizeof(int) * tribe->village_c);
    tribe->diff_table = allocate_memory(sizeof(int) * tribe->village_c * tribe->village_c);
    tribe->count_table = allocate_memory(sizeof(int) * tribe->village_c * tribe->village_c);
    tribe->cost_table = allocate_memory(sizeof(int) * tribe->village_c * tribe->village_c);
    tribe->sum_table = allocate_memory(sizeof(int) * tribe->village_c * tribe->village_c);

    for (int i = 0; i < tribe->village_c; ++i)
    {
        if (scanf(" %d", &tribe->villages[i]) != 1)
        {
            free_tribe(tribe);
            return NULL;
        }
        const int idx = get_idx(i, i, tribe->village_c);
        tribe->diff_table[idx] = 0;
        tribe->cost_table[idx] = 0;
        tribe->sum_table[idx] = 0;
        tribe->count_table[idx] = tribe->villages[i];
    }

    return tribe;
}

void free_tribe(tribe_t *tribe)
{
    deallocate_memory((void **)&tribe->diff_table);
    deallocate_memory((void **)&tribe->count_table);
    deallocate_memory((void **)&tribe->cost_table);
    deallocate_memory((void **)&tribe->sum_table);
    deallocate_memory((void **)&tribe->villages);
    deallocate_memory((void **)&tribe);
}

inline int get_idx(int r, int c, int village_count)
{
    return r * village_count + c;
}

int get_best_sum(tribe_t *t)
{
    return t->sum_table[t->village_c - 1];
}
