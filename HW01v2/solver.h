#ifndef __SOLVER_H__
#define __SOLVER_H__

#include "types.h"

/**
 * @brief Finds the largest suitable buildsite on given surface
 * 
 * @param surf surface to search
 * @return int number of tiles on largest suitable buildsites
 */
int solve(surface_t *surf);

#endif