#include "solver.h"

/**
 * @brief Updates the last rows on which a certain column can be reached from another 
 * 
 * @param r row to scan for column reachability
 * @param surf surface information
 */
void update_col_reachability(int r, surface_t *surf);

/**
 * @brief Scans a row and updates last blocked or uncertain tiles
 * 
 * @param r row to scan
 * @param surf surface information
 */
void update_rows(int r, surface_t *surf);

/**
 * @brief Scans positions with lower row on 'r' for possible buildsites
 * 
 * @param r last scanned row (lowest row of searched buildsites)
 * @param current_best curre
 * @param surf surface information
 * @return int - best found score
 */
int search_for_buildsites(int r, int current_best, surface_t *surf);

/**
 * @brief Returns height of column suitable for building with one uncertain tile
 * 
 * @param c column to get the height in
 * @param r last scanned row (row to get height from)
 * @param surf surface information
 * @return int number of suitable tiles
 */
int get_height_cost_one(int c, int r, surface_t *surf);

/**
 * @brief Returns height of column suitable for building without uncertain tile
 * 
 * @param c column to get the height in
 * @param r last scanned row (row to get height from)
 * @param surf surface information
 * @return int number of suitable tiles
 */
int get_height_cost_zero(int c, int r, surface_t *surf);

/**
 * @brief Returns boolean value of whether right column can be reached from left with one uncertain tile in between
 * 
 * @param min_row top-most row that can be used to reach the right column
 * @param left index of left column
 * @param right index of right column
 * @param surf surface information
 * @return int - 0 if not reachable, 1 otherwise
 */
int is_col_available_cost_one(int min_row, int left, int right, surface_t *surf);

/**
 * @brief Returns boolean value of whether right column can be reached from left without uncertain tile in between
 * 
 * @param min_row top-most row that can be used to reach the right column
 * @param left index of left column
 * @param right index of right column
 * @param surf surface information
 * @return int - 0 if not reachable, 1 otherwise
 */
int is_col_available_cost_zero(int min_row, int left, int right, surface_t *surf);

/**
 * @brief Gets number of columns that can be reached from a column in it's current suitable area
 * The number of columns is lowered by one (no point in scanning the closest column)
 * 
 * @param c column to reach from
 * @param surf surface information
 * @return int number of reachable columns lowered by 1
 */
int get_available_col_count(int c, surface_t *surf);

/**
 * @brief Return lower of the two numbers
 * 
 * @param n1 first number
 * @param n2 second number
 * @return int lower number
 */
int get_lower(int n1, int n2);

/**
 * @brief Searches the two columns for suitable buildsites on given row
 * 
 * @param r lowest row of the columns to scan
 * @param left left column of buildsite
 * @param right right column of buildsite
 * @param current_best currently_best score
 * @param surf surface information
 * @return int score of largest matched area
 */
int search_cols(int r, int left, int right, int current_best, surface_t *surf);

int solve(surface_t *surf)
{
    int current_best = 0;

    update_rows(0, surf);
    update_rows(1, surf);
    for (int r = 2; r < surf->rows; ++r)
    {
        update_rows(r, surf);
        update_col_reachability(r - 1, surf);
        int score = search_for_buildsites(r, current_best, surf);
        if (score > current_best)
            current_best = score;
    }

    return current_best;
}

void update_rows(int r, surface_t *surf)
{
    for (int c = 0; c < surf->cols; ++c)
    {
        switch (surf->map[r * surf->cols + c])
        {
        case 'X':
            surf->col_list[c].row_unusable = r;
            surf->col_list[c].row_uncertain = r;
            break;
        case 'o':
            surf->col_list[c].row_unusable = surf->col_list[c].row_uncertain;
            surf->col_list[c].row_uncertain = r;
            break;
        }
    }
}

void update_col_reachability(int r, surface_t *surf)
{
    int unusable = surf->cols - 1;
    int uncertain = surf->cols - 1;
    for (int c = surf->cols - 2; c > 0; --c)
    {
        switch (surf->map[r * surf->cols + c])
        {
        case 'X':
            unusable = c;
            uncertain = c;
            break;
        case 'o':
            unusable = uncertain;
            uncertain = c;
            break;
        }

        //write result to column one tile left of cursor
        for (int i = 0; i < uncertain - c; ++i)
        {
            surf->col_list[c - 1].cols_cost_zero[i] = r;
        }
        for (int i = 0; i < unusable - c; ++i)
        {
            surf->col_list[c - 1].cols_cost_one[i] = r;
        }
    }
}

int get_height_cost_one(int c, int r, surface_t *surf)
{
    return r - surf->col_list[c].row_unusable;
}

int get_height_cost_zero(int c, int r, surface_t *surf)
{
    return r - surf->col_list[c].row_uncertain;
}

int is_col_available_cost_one(int min_row, int left, int right, surface_t *surf)
{
    return surf->col_list[left].cols_cost_one[right - left - 2] >= min_row;
}

int is_col_available_cost_zero(int min_row, int left, int right, surface_t *surf)
{
    return surf->col_list[left].cols_cost_zero[right - left - 2] >= min_row;
}

int get_available_col_count(int c, surface_t *surf)
{
    int i;
    for (i = 0; i < surf->cols - c - 2; ++i)
    {
        if (surf->col_list[c].cols_cost_one[i] < surf->col_list[c].row_unusable + 2)
        {
            return i + 1;
        }
    }
    return i;
}

int get_lower(int n1, int n2)
{
    if (n1 < n2)
        return n1;
    else
        return n2;
}

int search_cols(int r, int left, int right, int current_best, surface_t *surf)
{
    const int left_zero = get_height_cost_zero(left, r, surf);
    const int right_zero = get_height_cost_zero(right, r, surf);
    const int left_one = get_height_cost_one(left, r, surf);
    const int right_one = get_height_cost_one(right, r, surf);

    //left 1 center 0 right 0
    int height = get_lower(right_zero, left_one);
    if (height > 2)
    {
        if (is_col_available_cost_zero(r - height + 2, left, right, surf))
        {
            int new_score = height * 2 + right - left - 1;
            if (new_score > current_best)
                current_best = new_score;
        }
    }

    //left 0 center 0 right 1
    height = get_lower(right_one, left_zero);
    if (height > 2)
    {
        if (is_col_available_cost_zero(r - height + 2, left, right, surf))
        {
            int new_score = height * 2 + right - left - 1;
            if (new_score > current_best)
                current_best = new_score;
        }
    }

    //left 0 center 1 right 0
    height = get_lower(right_zero, left_zero);
    if (height > 2)
    {
        if (is_col_available_cost_one(r - height + 2, left, right, surf))
        {
            int new_score = height * 2 + right - left - 1;
            if (new_score > current_best)
                current_best = new_score;
        }
    }

    return current_best;
}

int search_for_buildsites(int r, int current_best, surface_t *surf)
{
    for (int left = 0; left < surf->cols - 2; ++left)
    {
        if (get_height_cost_one(left, r, surf) < 3)
            continue;

        const int cols = get_available_col_count(left, surf);
        for (int i = 0; i < cols; ++i)
        {
            const int right = left + 2 + i;
            int new_score = search_cols(r, left, right, current_best, surf);
            if (new_score > current_best)
            {
                current_best = new_score;
            }
        }
    }
    return current_best;
}