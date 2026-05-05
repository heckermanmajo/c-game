//
// Created by mo on 05.05.26.
//

#ifndef CG_DRAW_UI_H
#define CG_DRAW_UI_H

#include <assert.h>

#include "../types.h"

void camp__draw_ui(Game *game)
{
    // header at the top
    {
        DrawRectangle(0, 0, GetScreenWidth(), 50, GRAY);
        DrawText("Next Round (1234)", 10, 10, 30, RED); // todo: add hover mode

        DrawText("Kraft: 123", 400, 10, 30, BLUE);
    }


    // draw some ui if there is a tile selected
    if (game->campaign_state.currently_selected_tile != NULL)
    {
        Color c = WHITE;
        c.a = 200;
        const int side_bar_width_in_pixels = 400;
        const int start_x_of_side_bar = GetScreenWidth() - side_bar_width_in_pixels;
        DrawRectangle(
            start_x_of_side_bar,
            50,
            side_bar_width_in_pixels,
            GetScreenHeight(),
            c
        );

        int y = 70;
        DrawText(
            TextFormat(
                "Tile %d|%d ",
                game->campaign_state.currently_selected_tile->x_index,
                game->campaign_state.currently_selected_tile->y_index
            ),
            start_x_of_side_bar + 10,
            y,
            20,
            BLACK
        );
        y += 30;
        DrawText(
            TextFormat(
                "Kraft per Round: %d ",
                game->campaign_state.currently_selected_tile->kraft_per_round
            ),
            start_x_of_side_bar + 10,
            y,
            20,
            BLACK
        );

        {
            char *type = "GRAS";
            switch (game->campaign_state.currently_selected_tile->terrain_type)
            {
                case CAMP_TERRAIN_TYPE_GRASS: type = "Grassland";
                    break;
                case CAMP_TERRAIN_TYPE_CITY: type = "City";
                    break;
                case CAMP_TERRAIN_TYPE_MOUNTAIN: type = "Mountain";
                    break;
                case CAMP_TERRAIN_TYPE_RESOURCE: type = "Resource";
                    break;
                case CAMP_TERRAIN_TYPE_WATER: type = "Water";
                    break;
            }
            y += 30;
            DrawText(
                type,
                start_x_of_side_bar + 10,
                y,
                20,
                BLACK
            );
        }

        if (game->campaign_state.currently_selected_tile->owner_faction)
        {
            y += 30;
            DrawText(
                TextFormat(
                    "Owner Faction: %s ",
                    game->campaign_state.currently_selected_tile->owner_faction->name
                ),
                start_x_of_side_bar + 10,
                y,
                20,
                game->campaign_state.currently_selected_tile->owner_faction->color
            );
        }

        if (game->campaign_state.currently_selected_tile->army.alive)
        {
            assert(game->campaign_state.currently_selected_tile->owner_faction);
            y += 30;
            DrawText(
                TextFormat(
                    "Army: %d ",
                    game->campaign_state.currently_selected_tile->army.kraft
                ),
                start_x_of_side_bar + 10,
                y,
                20,
                game->campaign_state.currently_selected_tile->owner_faction->color
            );
        }

        if (game->campaign_state.currently_selected_tile->terrain_type == CAMP_TERRAIN_TYPE_CITY)
        {
            // todo: add hover effect....
            y += 30;
            DrawText(
                "Click to create army: - 10 Kraft ",
                start_x_of_side_bar + 10,
                y,
                20,
                RED
            );
        }
    } // end drawing ui
}

#endif //CG_DRAW_UI_H
