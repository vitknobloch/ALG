#include "game.h"
#include "my_malloc.h"
#include <stdio.h>
#include <string.h>

int solve(game_t *game);
void inherit_row(game_t *game, int *prev_row, int *cur_row, int rowspan);
void update_row(game_t *game, int egg_index, int *cur_row);
int find_max_index(game_t *game, int *prev_row, int col_start, int col_end);

int main(int argc, char *argv[])
{
    game_t *game = load_game();
    sort_eggs(game);

    printf("%d\r\n", solve(game));

    free_game(game);

    return 0;
}

int solve(game_t *game)
{
    int *prev_row = allocate_memory(sizeof(int) * (game->cols - 1));
    int *cur_row = allocate_memory(sizeof(int) * (game->cols - 1));

    memset(prev_row, 0, sizeof(int) * (game->cols - 1));
    memset(cur_row, 0, sizeof(int) * (game->cols - 1));

    int egg_index = game->egg_count - 1;
    int last_non_empty = game->rows;
    for (int r = game->rows; r >= 0; --r)
    {
        if (egg_index >= 0 && game->eggs[egg_index].row < r)
            continue;
        inherit_row(game, prev_row, cur_row, last_non_empty - r);
        while (egg_index >= 0 && game->eggs[egg_index].row == r)
        {
            update_row(game, egg_index, cur_row);
            --egg_index;
        }
        int *temp = prev_row;
        prev_row = cur_row;
        cur_row = temp;
        last_non_empty = r;
    }

    inherit_row(game, prev_row, cur_row, last_non_empty + 1);

    int ret = cur_row[0];
    deallocate_memory((void **)&prev_row);
    deallocate_memory((void **)&cur_row);

    return ret;
}

int find_max_index(game_t *game, int *prev_row, int col_start, int col_end)
{
    col_start = col_start < 0 ? 0 : col_start;
    col_end = col_end > game->cols - 2 ? game->cols - 2 : col_end;
    int max_index = col_start;
    for (int c = col_start + 1; c <= col_end; ++c)
    {
        if (prev_row[c] >= prev_row[max_index])
            max_index = c;
    }
    return max_index;
}

void inherit_row(game_t *game, int *prev_row, int *cur_row, int rowspan)
{
    int max_index = find_max_index(game, prev_row, 0, rowspan);
    for (int c = 0; c < game->cols - 1; ++c)
    {
        if (c + rowspan < game->cols - 1 && prev_row[c + rowspan] >= prev_row[max_index])
            max_index = c + rowspan;
        if (max_index < c - rowspan)
            max_index = find_max_index(game, prev_row, c - rowspan, c + rowspan);
        cur_row[c] = prev_row[max_index];
    }
}

void update_row(game_t *game, int egg_index, int *cur_row)
{
    int col = game->eggs[egg_index].col;
    if (col < game->cols - 1)
        cur_row[col]++;
    if (col > 0)
        cur_row[col - 1]++;
}
