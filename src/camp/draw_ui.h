//
// Created by mo on 05.05.26.
//

#ifndef CG_DRAW_UI_H
#define CG_DRAW_UI_H

#include <assert.h>

#include "progress_to_next_round.h"
#include "../types.h"

void camp__draw_ui(Game *game)
{
    // header at the top
    {
        DrawRectangle(0, 0, GetScreenWidth(), 50, GRAY);

        // progress round button
        {
            const char *text = TextFormat(
                "Next Round (%d)",
                game->campaign_state.current_round
            );
            const int font_size = 30;
            if (text_button(text, 10, 10, font_size, PURPLE))
            {
                camp__progress_to_next_round(game);
            }
        }

        DrawText(
            TextFormat(
                "Kraft: %d",
                game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft
            ),
            400,
            10,
            30,
            BLUE
        );

        DrawText(
            TextFormat(
                "Kraft(enemy): %d",
                game->campaign_state.factions[1].kraft
            ),
            700,
            10,
            30,
            RED
        );
    }


    // draw some ui if there is a tile selected
    if (game->campaign_state.currently_selected_tile != NULL)
    {
        Color c = GRAY;
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
            char *type = "UNKNOWN";
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
                case CAMP_TERRAIN_TYPE_LOGISTICS_HUB: type = "Logistics Hub";
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

        if (game->campaign_state.currently_selected_tile->owner_faction == &game->campaign_state.factions[
                PLAYER_FACTION_INDEX])
        {
            if (game->campaign_state.currently_selected_tile->terrain_type == CAMP_TERRAIN_TYPE_CITY)
            {
                y += 30;
                const int font_size = 20;
                const int x = start_x_of_side_bar + 10;
                const int price = 10;
                if (game->campaign_state.currently_selected_tile->army.alive)
                {
                    if (text_button(TextFormat("Click to enforce army: - %d Kraft ", price), x, y, font_size, PURPLE))
                    {
                        if (game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft > 10)
                        {
                            game->campaign_state.currently_selected_tile->army.kraft += 10;
                            game->campaign_state.currently_selected_tile->army.can_move_this_turn = 0;
                            game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft -= 10;
                        }
                    }
                } else
                {
                    if (text_button(TextFormat("Click to CREATE army: - %d Kraft ", price), x, y, font_size, PURPLE))
                    {
                        if (game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft > 10)
                        {
                            game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft -= 10;
                            game->campaign_state.currently_selected_tile->army.kraft = 10;
                            game->campaign_state.currently_selected_tile->army.alive = 1;
                            game->campaign_state.currently_selected_tile->army.tile_i_am_on = game->campaign_state.
                                    currently_selected_tile;
                            game->campaign_state.currently_selected_tile->army.faction = &game->campaign_state.factions[
                                PLAYER_FACTION_INDEX];
                            game->campaign_state.currently_selected_tile->army.can_move_this_turn = 0;
                        }
                    }
                }
            }

            if (game->campaign_state.currently_selected_tile->terrain_type == CAMP_TERRAIN_TYPE_LOGISTICS_HUB)
            {
                y += 30;
                const int font_size = 20;
                const int x = start_x_of_side_bar + 10;
                const int price = 10;
                if (game->campaign_state.currently_selected_tile->army.alive)
                {
                    if (text_button(TextFormat("Click to enforce army: - %d Kraft ", price), x, y, font_size, PURPLE))
                    {
                        if (game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft > 10)
                        {
                            game->campaign_state.currently_selected_tile->army.kraft += 10;
                            game->campaign_state.currently_selected_tile->army.can_move_this_turn = 0;
                            game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft -= 10;
                        }
                    }
                }
            }

            if (game->campaign_state.currently_selected_tile->terrain_type == CAMP_TERRAIN_TYPE_GRASS)
            {
                y += 30;
                const int font_size = 20;
                const int x = start_x_of_side_bar + 10;
                const int price = 30;
                if (
                    game->campaign_state.currently_selected_tile->army.alive
                    && game->campaign_state.currently_selected_tile->army.can_move_this_turn == 1
                )
                {
                    if (text_button(TextFormat("Click to build Logistics hub: - %d Kraft ", price), x, y, font_size,
                                    PURPLE))
                    {
                        if (game->campaign_state.factions[PLAYER_FACTION_INDEX].kraft > price)
                        {
                            game->campaign_state.currently_selected_tile->army.can_move_this_turn = 0;
                            game->campaign_state.currently_selected_tile->terrain_type =
                                    CAMP_TERRAIN_TYPE_LOGISTICS_HUB;
                        }
                    }
                }
            }
        }
    } // end drawing ui
}

#endif //CG_DRAW_UI_H
