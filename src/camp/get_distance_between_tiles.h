//
// Created by mo on 05.05.26.
//

#ifndef CG_GET_DISTANCE_BETWEEN_TILES_H
#define CG_GET_DISTANCE_BETWEEN_TILES_H

#include <math.h>

#include "../types.h"

float get_distance_between_tiles( CampTile* from, CampTile* to);

#endif //CG_GET_DISTANCE_BETWEEN_TILES_H


#ifdef IMPLEMENTATION


float get_distance_between_tiles( CampTile* from, CampTile* to)
{
    // a**2 + b**2 = c**2
    return sqrt(
        abs(from->x_index - to->x_index) * abs(from->x_index - to->x_index)
        + abs(from->y_index - to->y_index) * abs(from->y_index - to->y_index)
    );
}

#endif
