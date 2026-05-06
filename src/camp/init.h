//
// Created by mo on 03.05.26.
//

#ifndef CG_INIT_H
#define CG_INIT_H

#include <raylib.h>

#include "../types.h"

void camp__init(Game *game)
{

    // init log files
    {
        game->campaign_state.log_file = fopen("../camp_file.log", "w");
        if (game->campaign_state.log_file == NULL)
        {
            printf("cannot open file for logs");
            exit(EXIT_FAILURE);
        }
    }

    // set default stuff
    {
        game->campaign_state.merge_overlay_state.from = NULL;
        game->campaign_state.merge_overlay_state.to = NULL;
        game->campaign_state.battle_overlay_state.from = NULL;
        game->campaign_state.battle_overlay_state.to = NULL;
        game->campaign_state.overlay_mode = CAMP_OVERLAY_NONE;

        game->campaign_state.currently_selected_tile = NULL;
        game->campaign_state.camp_tiles_around.success = 0;
        game->campaign_state.debug_mode = DEBUG_MODE_NONE;
    }

    // init factions
    {
        game->campaign_state.factions[0].is_player = 1;
        game->campaign_state.factions[0].name = "Player Faction";
        game->campaign_state.factions[0].color = BLUE;
        game->campaign_state.factions[0].kraft = 0;

        game->campaign_state.factions[1].is_player = 0;
        game->campaign_state.factions[1].name = "Enemy Faction";
        game->campaign_state.factions[1].color = RED;
        game->campaign_state.factions[1].kraft = 0;
        LOG(&game->campaign_state, "Initialized factions");
    }

    // init campaign map
    {
        for (int i_x = 0; i_x < CAMP_MAP_SIZE; i_x++)
        {
            for (int i_y = 0; i_y < CAMP_MAP_SIZE; i_y++)
            {
                const int index = i_x * CAMP_MAP_SIZE + i_y;
                CampTile *tile = &(game->campaign_state.tiles[index]);
                tile->x_index = i_x;
                tile->y_index = i_y;
                tile->x_world = i_x * CAMP_TILE_SIZE_IN_PIXELS;
                tile->y_world = i_y * CAMP_TILE_SIZE_IN_PIXELS;
                tile->army.alive = 0;
                tile->army.kraft = 0;
                tile->army.faction = NULL;
                tile->army.tile_i_am_on = NULL;
                {
                    const int rand = GetRandomValue(0, 100);
                    if (rand > 95)
                    {
                        tile->terrain_type = CAMP_TERRAIN_TYPE_CITY;
                        tile->kraft_per_round = 5;
                        if (GetRandomValue(0, 1) == 0)
                        {
                            tile->owner_faction = &(game->campaign_state.factions[0]);
                        } else
                        {
                            tile->owner_faction = &(game->campaign_state.factions[1]);
                        }
                    } else if (rand > 90)
                    {
                        tile->terrain_type = CAMP_TERRAIN_TYPE_RESOURCE;
                        tile->kraft_per_round = 10;
                        if (GetRandomValue(0, 1) == 0)
                        {
                            tile->owner_faction = &(game->campaign_state.factions[0]);
                        } else
                        {
                            tile->owner_faction = &(game->campaign_state.factions[1]);
                        }
                    } else if (rand > 85)
                    {
                        tile->terrain_type = CAMP_TERRAIN_TYPE_MOUNTAIN;
                        tile->kraft_per_round = 0;
                    } else if (rand > 80)
                    {
                        tile->terrain_type = CAMP_TERRAIN_TYPE_WATER;
                        tile->kraft_per_round = 0;
                    } else if (rand > 80) // currently disabled
                    {
                        tile->terrain_type = CAMP_TERRAIN_TYPE_LOGISTICS_HUB;
                        tile->kraft_per_round = 2;
                        if (GetRandomValue(0, 1) == 0)
                        {
                            tile->owner_faction = &(game->campaign_state.factions[0]);
                        } else
                        {
                            tile->owner_faction = &(game->campaign_state.factions[1]);
                        }
                    } else
                    {
                        tile->terrain_type = CAMP_TERRAIN_TYPE_GRASS;
                        tile->kraft_per_round = 0;
                        if (GetRandomValue(0, 1) == 0)
                        {
                            tile->owner_faction = &(game->campaign_state.factions[0]);
                        } else
                        {
                            tile->owner_faction = &(game->campaign_state.factions[1]);
                        }
                    }
                }
                if (
                    tile->terrain_type == CAMP_TERRAIN_TYPE_GRASS
                )
                {
                    const int rand = GetRandomValue(0, 100);
                    if (rand > 90)
                    {
                        tile->owner_faction = &(game->campaign_state.factions[0]);
                        tile->army.alive = 1;
                        tile->army.kraft = 100;
                        tile->army.faction = &(game->campaign_state.factions[0]);
                        tile->army.tile_i_am_on = tile;
                        tile->army.can_move_this_turn = 1;
                    } else if (rand > 80)
                    {
                        tile->owner_faction = &(game->campaign_state.factions[1]);
                        tile->army.alive = 1;
                        tile->army.kraft = 100;
                        tile->army.faction = &(game->campaign_state.factions[1]);
                        tile->army.tile_i_am_on = tile;
                        tile->army.can_move_this_turn = 1;
                    }
                }
            }
        }
    }

    // init camera
    {
        game->campaign_state.camera.offset
                = (Vector2){
                    .x = ((float) GetScreenWidth()) / 2,
                    .y = ((float) GetScreenHeight()) / 2,
                };
        game->campaign_state.camera.target
                = (Vector2){
                    .x = ((float) GetScreenWidth()) / 2,
                    .y = ((float) GetScreenHeight()) / 2,
                };
        game->campaign_state.camera.rotation = 0.0f;
        game->campaign_state.camera.zoom = 1.0f;
    }
}
#endif //CG_INIT_H
