//
// Created by mo on 05.05.26.
//

#ifndef CG_USER_INPUT_CONTROLLER_H
#define CG_USER_INPUT_CONTROLLER_H

#include "../types.h"

void camp__user_input_controller(Game *game)
{
    if (game->campaign_state.overlay_mode != CAMP_OVERLAY_NONE)
    {
        return;
    }

    // move player armies with arrow keys
    // todo: add more comments
    if (game->campaign_state.currently_selected_tile && game->campaign_state.currently_selected_tile->owner_faction)
    {
        if (game->campaign_state.currently_selected_tile->owner_faction->is_player)
        {
            if (IsKeyPressed(KEY_UP))
            {
                CampTilesAroundResult above_result = get_camp_tiles_around(
                    game,
                    game->campaign_state.currently_selected_tile,
                    (CampTilesAroundFilter){
                        .only_top = 1,
                        .depth = 1
                    }
                );

                if (above_result.success == 0)
                {
                    printf("%s", above_result.reason);
                    exit(0);
                }

                if (above_result.amount == 1)
                {
                    CampTile *above_tile = above_result.malloced_tiles[0];
                    move_army(game, game->campaign_state.currently_selected_tile, above_tile);
                }

                camp_tiles_around_result_destroy(&above_result);
            }

            if (IsKeyPressed(KEY_DOWN))
            {
                CampTilesAroundResult above_result = get_camp_tiles_around(
                    game,
                    game->campaign_state.currently_selected_tile,
                    (CampTilesAroundFilter){
                        .only_bottom = 1,
                        .depth = 1
                    }
                );

                if (above_result.success == 0)
                {
                    printf("%s", above_result.reason);
                    exit(0);
                }

                if (above_result.amount == 1)
                {
                    CampTile *above_tile = above_result.malloced_tiles[0];
                    move_army(game, game->campaign_state.currently_selected_tile, above_tile);
                }

                camp_tiles_around_result_destroy(&above_result);
            }

            if (IsKeyPressed(KEY_LEFT))
            {
                CampTilesAroundResult above_result = get_camp_tiles_around(
                    game,
                    game->campaign_state.currently_selected_tile,
                    (CampTilesAroundFilter){
                        .depth = 1,
                        .only_left = 1
                    }
                );

                if (above_result.success == 0)
                {
                    printf("%s", above_result.reason);
                    exit(0);
                }

                if (above_result.amount == 1)
                {
                    CampTile *above_tile = above_result.malloced_tiles[0];
                    move_army(game, game->campaign_state.currently_selected_tile, above_tile);
                }

                camp_tiles_around_result_destroy(&above_result);
            }

            if (IsKeyPressed(KEY_RIGHT))
            {
                CampTilesAroundResult above_result = get_camp_tiles_around(
                    game,
                    game->campaign_state.currently_selected_tile,
                    (CampTilesAroundFilter){
                        .depth = 1,
                        .only_right = 1
                    }
                );

                if (above_result.success == 0)
                {
                    printf("%s", above_result.reason);
                    exit(0);
                }

                if (above_result.amount == 1)
                {
                    CampTile *above_tile = above_result.malloced_tiles[0];
                    move_army(game, game->campaign_state.currently_selected_tile, above_tile);
                }

                camp_tiles_around_result_destroy(&above_result);
            }
        }
    }

    // select tile on camp map via mouse
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        mouse = GetScreenToWorld2D(mouse, game->campaign_state.camera);
        const int mouse_in_world = CheckCollisionPointRec(
            mouse,
            (Rectangle){
                .x = 0, .y = 0, .width = CAMP_SIZE_IN_PIXELS,
                .height = CAMP_SIZE_IN_PIXELS
            }
        );
        if (mouse_in_world)
        {
            const int index_of_camp_tile_x = (int) (mouse.x / CAMP_TILE_SIZE_IN_PIXELS);
            const int index_of_camp_tile_y = (int) (mouse.y / CAMP_TILE_SIZE_IN_PIXELS);
            const int index = index_of_camp_tile_x * CAMP_MAP_SIZE + index_of_camp_tile_y;
            CampTile *tile = &(game->campaign_state.tiles[index]);
            //printf("mouse in world: %d", index);
            game->campaign_state.currently_selected_tile = tile;

            if (tile && game->campaign_state.debug_mode == DEBUG_MODE_GET_CAMP_TILES_AROUND)
            {
                camp_tiles_around_result_destroy(&game->campaign_state.camp_tiles_around);
                CampTilesAroundResult result = get_camp_tiles_around(
                    game,
                    tile,
                    (CampTilesAroundFilter){
                        .depth = 2,
                        .only_top = 1
                    }
                );
                if (result.success == 0)
                {
                    printf("%s", result.reason);
                    exit(0);
                } else
                {
                    //printf("success got around: %d", result.amount);
                }
                game->campaign_state.camp_tiles_around = result;
            }
        }
    }

    // progress to next round
    if (IsKeyPressed(KEY_ENTER))
    {
        camp__progress_to_next_round(game);
    }
}

#endif //CG_USER_INPUT_CONTROLLER_H
