//
// Created by mo on 03.05.26.
//

#ifndef CG_DRAW_H
#define CG_DRAW_H

#include "raylib.h"
#include "../types.h"

void camp__draw_world(Game *game)
{
    for (int i_x = 0; i_x < CAMP_MAP_SIZE; i_x++)
    {
        for (int i_y = 0; i_y < CAMP_MAP_SIZE; i_y++)
        {
            const int index = i_x * CAMP_MAP_SIZE + i_y;
            const CampTile *tile = &(game->campaign_state.tiles[index]);
            switch (tile->terrain_type)
            {
                case CAMP_TERRAIN_TYPE_GRASS:
                    DrawRectangle(
                        tile->x_world,
                        tile->y_world,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        GREEN
                    );
                    break;
                case CAMP_TERRAIN_TYPE_WATER:
                    DrawRectangle(
                        tile->x_world,
                        tile->y_world,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        BLUE
                    );
                    break;
                case CAMP_TERRAIN_TYPE_CITY:
                {
                    DrawRectangle(
                        tile->x_world,
                        tile->y_world,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        GREEN
                    );
                    DrawText("CITY", tile->x_world + 10, tile->y_world + 10, 10, BLACK);
                    int center_x = tile->x_world + CAMP_TILE_SIZE_IN_PIXELS / 2 - 4;
                    int center_y = tile->y_world + CAMP_TILE_SIZE_IN_PIXELS / 2 - 4;
                    Color red = RED;
                    red.b = 100;
                    red.g = 100;
                    DrawCircle(
                        center_x,
                        center_y,
                        CAMP_TILE_SIZE_IN_PIXELS / 2 - 20,
                        red
                    );
                }
                break;
                case CAMP_TERRAIN_TYPE_RESOURCE:
                    DrawRectangle(
                        tile->x_world,
                        tile->y_world,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        GREEN
                    );
                    int center_x = tile->x_world + CAMP_TILE_SIZE_IN_PIXELS / 2 - 4;
                    int center_y = tile->y_world + CAMP_TILE_SIZE_IN_PIXELS / 2 - 4;
                    Color brown = BROWN;
                    brown.b = 100;
                    brown.g = 100;
                    DrawCircle(
                        center_x,
                        center_y,
                        CAMP_TILE_SIZE_IN_PIXELS / 2 - 20,
                        brown
                    );
                    DrawText("RES", tile->x_world + 10, tile->y_world + 10, 10, BLACK);
                    break;
                case CAMP_TERRAIN_TYPE_MOUNTAIN:
                    DrawRectangle(
                        tile->x_world,
                        tile->y_world,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                        GRAY
                    );
                    break;
            }

            Color c = WHITE;
            if (tile->owner_faction != NULL)
            {
                c = tile->owner_faction->color;
            }
            // debug code to check that the around-function does as expected
            if (game->campaign_state.currently_selected_tile && game->campaign_state.camp_tiles_around.success && game->
                campaign_state.debug_mode == DEBUG_MODE_GET_CAMP_TILES_AROUND)
            {
                for (int i = 0; i < game->campaign_state.camp_tiles_around.amount; i++)
                {
                    const CampTile *around_tile = game->campaign_state.camp_tiles_around.malloced_tiles[i];
                    if (tile->x_index == around_tile->x_index && tile->y_index == around_tile->y_index)
                    {
                        c = PURPLE;
                        Color c2 = PURPLE;
                        c2.a = 80;
                        DrawRectangle(
                            tile->x_world,
                            tile->y_world,
                            CAMP_TILE_SIZE_IN_PIXELS - 4,
                            CAMP_TILE_SIZE_IN_PIXELS - 4,
                            c2
                        );
                    }
                }
            }

            if (tile == game->campaign_state.currently_selected_tile)
            {
                c = YELLOW;
            }

            DrawRectangleLinesEx(
                (Rectangle){
                    .x = (float) tile->x_world,
                    .y = (float) tile->y_world,
                    .width = (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                    .height = (int) CAMP_TILE_SIZE_IN_PIXELS - 4,
                },
                5,
                c
            );

            // draw army on tile
            {
                if (tile->army.alive)
                {
                    DrawTriangle(
                        (Vector2){
                            .x = tile->x_world + CAMP_TILE_SIZE_IN_PIXELS / 2,
                            .y = tile->y_world + 20
                        },
                        (Vector2){
                            .x = tile->x_world + 20,
                            .y = tile->y_world + CAMP_TILE_SIZE_IN_PIXELS - 20
                        },
                        (Vector2){
                            .x = tile->x_world + CAMP_TILE_SIZE_IN_PIXELS - 20,
                            .y = tile->y_world + CAMP_TILE_SIZE_IN_PIXELS - 20
                        },
                        tile->army.faction->color
                    );

                    if (tile->army.can_move_this_turn == 0)
                    {
                        Color gray = GRAY;
                        gray.a = 90;
                        DrawTriangle(
                            (Vector2){
                                .x = tile->x_world + CAMP_TILE_SIZE_IN_PIXELS / 2,
                                .y = tile->y_world + 20
                            },
                            (Vector2){
                                .x = tile->x_world + 20,
                                .y = tile->y_world + CAMP_TILE_SIZE_IN_PIXELS - 20
                            },
                            (Vector2){
                                .x = tile->x_world + CAMP_TILE_SIZE_IN_PIXELS - 20,
                                .y = tile->y_world + CAMP_TILE_SIZE_IN_PIXELS - 20
                            },
                            gray
                        );
                    }

                    {
                        Color c = WHITE;
                        if (tile->army.can_move_this_turn == 0)
                        {
                            c = LIGHTGRAY;
                        }
                        DrawText(
                            TextFormat("%d", tile->army.kraft),
                            tile->x_world + 40,
                            tile->y_world + CAMP_TILE_SIZE_IN_PIXELS - 40,
                            20,
                            c
                        );
                    }
                }
            } // end draw armies on tiles
        } // inner for loop all camp tiles
    } // outer for loop all camp tiles
}

#endif //CG_DRAW_H
