#ifndef __LOADER_H__
#define __LOADER_H__

#include "types.h"
/**
 * @brief Loads surface from stdin and calculates sum arrays
 * 
 * @return surface_t loaded surface
 */
surface_t load_surface();

/**
 * @brief Frees memory alocated for surface sum arrays
 * 
 * @param surface pointer to surface struct to free
 */
void free_surface(surface_t *surface);

#endif