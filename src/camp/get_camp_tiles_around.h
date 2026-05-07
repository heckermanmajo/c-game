// majo - @hackermanmajo@gmail.com

#ifndef CG_GET_CAMP_TILES_AROUND_H
#define CG_GET_CAMP_TILES_AROUND_H

#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../types.h"
#include "../types.h"

typedef struct CampTilesAroundResult CampTilesAroundResult;
typedef struct CampTilesAroundFilter CampTilesAroundFilter;

// some forward definitions
struct CampTile;
struct Game;

#define CampTilesAroundResult_REASON_BUFFER_SIZE 256

/**
 * Result struct for get_camp_tiles_around()
 * @see get_camp_tiles_around()
 */
struct CampTilesAroundResult
{
    /** 0 if it failed, 1 if it succeeded */
    int success;
    /** contains a reason why it failed - so useless if success is 1 */
    char reason[CampTilesAroundResult_REASON_BUFFER_SIZE];
    /** The amount of tiles in "malloced_tiles" - for looping over them. */
    int amount;
    /**
     * The array of pointers to the tiles; Pointers to them, so you can use
     * those pointers to modify the real data.
     */
    struct CampTile **malloced_tiles;
};

/**
 * Filter params for get_camp_tiles_around.
 * @see get_camp_tiles_around
 */
struct CampTilesAroundFilter
{
    // todo: add more filters
    //int only_passable;
    //int only_free;
    // int only_no_army;
    //int directly_reachable;
    //int also_diagonal;
    int only_left;
    int only_right;
    int only_top;
    int only_bottom;

    int only_passable;

    /**
     * Determines how big the square around the start tile to return.
     * 1 == 3x3
     * 2 == 5x5
     * 3 == 7x7
     * 4 == 9x9
     * 5 == 11x11
     * 6 == 13x13
     *
     * 6 is the maximum.
     *
     * Other filters are applied after this one is applied.
     */
    unsigned int depth;
};


/**
 * Helper-function for working with the campaign map.
 *
 * Returns tiles around the given start_tile.
 *
 * The settings for what tiles to return can be set in the filter.
 *
 * NOTE: THE RESULT MAY CONTAIN MALLOCED MEMORY
 * @see camp_tiles_around_result_destroy
 * @see CampTilesAroundFilter
 * @see CampTilesAroundResult
 *
 * @param game pointer to a valid game state
 * @param start_tile the middle tile you want to get the tiles around.
 * @param filter struct with filter settings
 * @return the result with the tiles around the start tile, based on filter settings
 */
CampTilesAroundResult get_camp_tiles_around(
    struct Game *game,
    const struct CampTile *start_tile,
    CampTilesAroundFilter filter
);


/**
 * Helper function to free the result without headache.
 * @param result the ptr to the result to free
 */
void camp_tiles_around_result_destroy(CampTilesAroundResult *result);


#endif


//////////////////////////////////////////////////////////////////
// IMPLEMENTATION
/////////////////////////////////////////////////////////////////


#ifdef IMPLEMENTATION

CampTilesAroundResult get_camp_tiles_around(
    Game *game,
    const CampTile *start_tile,
    const CampTilesAroundFilter filter
)
{
    CampTilesAroundResult result;
    result.success = 0;
    result.amount = 0;
    memset(result.reason, 0, CampTilesAroundResult_REASON_BUFFER_SIZE);
    result.malloced_tiles = NULL;

    if (!start_tile)
    {
        snprintf(
            result.reason,
            CampTilesAroundResult_REASON_BUFFER_SIZE,
            "start_tile is NULL"
        );
        return result;
    }

    if (!game)
    {
        snprintf(
            result.reason,
            CampTilesAroundResult_REASON_BUFFER_SIZE,
            "game is NULL"
        );
        return result;
    }

    const unsigned int max_depth = 6;
    if (filter.depth > max_depth)
    {
        snprintf(
            result.reason,
            CampTilesAroundResult_REASON_BUFFER_SIZE,
            "Depth is bigger than max depth; %u is bigger than %u (max)",
            filter.depth,
            max_depth
        );
        return result;
    }

    if (filter.depth < 1)
    {
        snprintf(
            result.reason,
            CampTilesAroundResult_REASON_BUFFER_SIZE,
            "Depth is smaller  than min depth; %u is smaller than 1 (min)",
            filter.depth
        );
        return result;
    }

    // end validation

    int max_amount = 0;
    // determine the maximum needed memory
    {
        switch (filter.depth)
        {
            case 1: max_amount = 3 * 3;
                break;
            case 2: max_amount = 5 * 5;
                break;
            case 3: max_amount = 7 * 7;
                break;
            case 4: max_amount = 9 * 9;
                break;
            case 5: max_amount = 11 * 11;
                break;
            case 6: max_amount = 13 * 13;
                break;
        }
        max_amount--; // since we don't want to include the start tile
    }

    // allocate needed memory
    // might allocate more memory than needed, since we might apply other filters
    // but it is fine, since the amount of memory is very small in all cases
    {
        const size_t bytes_to_allocate = sizeof(CampTile *) * (size_t) max_amount;
        result.malloced_tiles = malloc(bytes_to_allocate);
        if (!result.malloced_tiles)
        {
            snprintf(
                result.reason,
                CampTilesAroundResult_REASON_BUFFER_SIZE,
                "Allocation of %zu bytes failed",
                bytes_to_allocate
            );
            return result;
        }
    }

    // fill in the tiles with the tiles around the start tile
    {
        const int start_index_x = start_tile->x_index - filter.depth;
        const int end_index_x = start_tile->x_index + filter.depth + 1;
        const int start_index_y = start_tile->y_index - filter.depth;
        const int end_index_y = start_tile->y_index + filter.depth + 1;

        for (int x = start_index_x; x < end_index_x; x++)
        {
            if (x < 0) continue;
            if (x > CAMP_MAP_SIZE - 1) continue;
            for (int y = start_index_y; y < end_index_y; y++)
            {
                if (y < 0) continue;
                if (y > CAMP_MAP_SIZE - 1) continue;
                const int index = x * CAMP_MAP_SIZE + y;
                CampTile *tile = &game->campaign_state.tiles[index];

                // ignore the start tile itself
                if (x == start_tile->x_index && y == start_tile->y_index) continue;


                // todo: MOVE COMMENTS
                // filter only up, left, right, bottom
                if (filter.only_top == 1)
                {
                    if (tile->x_index != start_tile->x_index)
                    {
                        continue;
                    }
                    if (tile->y_index > start_tile->y_index)
                    {
                        continue;
                    }
                }

                if (filter.only_bottom == 1)
                {
                    if (tile->x_index != start_tile->x_index)
                    {
                        continue;
                    }
                    if (tile->y_index < start_tile->y_index)
                    {
                        continue;
                    }
                }

                if (filter.only_left == 1)
                {
                    if (tile->y_index != start_tile->y_index)
                    {
                        continue;
                    }
                    if (tile->x_index > start_tile->x_index)
                    {
                        continue;
                    }
                }

                if (filter.only_right == 1)
                {
                    if (tile->y_index != start_tile->y_index)
                    {
                        continue;
                    }
                    if (tile->x_index < start_tile->x_index)
                    {
                        continue;
                    }
                }

                if (filter.only_passable == 1)
                {
                    if (
                        tile->terrain_type == CAMP_TERRAIN_TYPE_WATER
                        || tile->terrain_type == CAMP_TERRAIN_TYPE_MOUNTAIN
                    )
                    {
                        continue;
                    }
                }

                result.malloced_tiles[result.amount] = tile;
                result.amount++;
            }
        }
    }

    //printf( "GOT %d around tiles", result.amount );

    result.success = 1;

    return result;
}

void camp_tiles_around_result_destroy(CampTilesAroundResult *result)
{
    // todo: we might want to add logs here -> if we pass in a null or the emory is already freed

    if (!result)
    {
        return;
    }

    if (result->malloced_tiles)
    {
        free(result->malloced_tiles);
    }

    result->malloced_tiles = NULL;
    result->amount = 0;
    result->success = 0;
    result->reason[0] = '\0';
}

#endif
