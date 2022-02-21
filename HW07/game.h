#ifndef __GAME_H__
#define __GAME_H__

typedef struct
{
    int row;
    int col;
} egg_t;

typedef struct
{
    int cols;
    int rows;
    int egg_count;
    egg_t *eggs;
} game_t;

game_t *load_game();
void free_game(game_t *game);

void sort_eggs(game_t *game);

#endif