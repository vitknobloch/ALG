#include "game.h"
#include "my_malloc.h"

#include <stdio.h>
#include <string.h>

game_t *load_game()
{
    game_t *game = allocate_memory(sizeof(game_t));
    game->rows = 0;
    scanf("%d %d", &game->cols, &game->egg_count);
    game->eggs = allocate_memory(sizeof(egg_t) * game->egg_count);
    for (int i = 0; i < game->egg_count; ++i)
    {
        egg_t *egg = &game->eggs[i];
        scanf("%d %d", &egg->col, &egg->row);
        egg->col--;
        egg->row--;
        if (egg->row > game->rows)
            game->rows = egg->row;
    }
    game->rows++;
    return game;
}

void free_game(game_t *game)
{
    deallocate_memory((void **)&game->eggs);
    deallocate_memory((void **)&game);
}

void sort_eggs(game_t *game)
{
    //allocate sorting structs
    int *counter = allocate_memory(sizeof(int) * game->rows);
    egg_t *sorted = allocate_memory(sizeof(egg_t) * game->egg_count);
    memset(counter, 0, sizeof(int) * game->rows);
    //calculate number of eggs in each row
    for (int i = 0; i < game->egg_count; ++i)
    {
        counter[game->eggs[i].row]++;
    }
    //calculate index of latest egg in the row
    int sum = -1;
    for (int i = 0; i < game->rows; ++i)
    {
        sum += counter[i];
        counter[i] = sum;
    }
    //fill sorted array
    for (int i = game->egg_count - 1; i >= 0; --i)
    {
        sorted[counter[game->eggs[i].row]] = game->eggs[i];
        --counter[game->eggs[i].row];
    }
    //replace and free eggs array
    deallocate_memory((void **)&counter);
    deallocate_memory((void **)&game->eggs);
    game->eggs = sorted;
}