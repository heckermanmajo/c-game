//
// Created by mo on 05.05.26.
//

#ifndef CG_DRAW_OVERLAY_H
#define CG_DRAW_OVERLAY_H

#include "../types.h"


void camp__draw_overlay(Game *game)
{
    if (game->campaign_state.overlay_mode == CAMP_OVERLAY_NONE)
    {
        return;
    }

    switch (game->campaign_state.overlay_mode)
    {
        case CAMP_OVERLAY_MENU:
        {
            DrawRectangle(
                100,
                100,
                GetScreenHeight() - 100,
                GetScreenWidth() - 100,
                BLACK
            );
            // todo: real logic here ...
            DrawText("IN CAMP MENU", 200, 200, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER))
            {
                game->campaign_state.overlay_mode = CAMP_OVERLAY_NONE;
            }
        }
        break;

        case CAMP_OVERLAY_BATTLE:
        {
            DrawRectangle(
                100,
                100,
                GetScreenHeight() - 100,
                GetScreenWidth() - 100,
                BLACK
            );
            // todo: real logic here ...
            // if battle button is clicked -> go to rts-battle mode

            DrawText("CAMP OVERLAY BATTLE", 200, 200, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER))
            {
                game->campaign_state.overlay_mode = CAMP_OVERLAY_NONE;
            }
        }
        break;

        case CAMP_OVERLAY_MERGE:
        {
            DrawRectangle(
                100,
                100,
                GetScreenHeight() - 100,
                GetScreenWidth() - 100,
                BLACK
            );
            // todo: real logic here ...
            DrawText("TWO ARMY MERGE MENU", 200, 200, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER))
            {
                game->campaign_state.overlay_mode = CAMP_OVERLAY_NONE;
            }
        }
        break;

        // this is opened after the auto-battle OR after a real rts battle
        case CAMP_OVERLAY_BATTLE_RESULT:
        {
            DrawRectangle(
                100,
                100,
                GetScreenHeight() - 100,
                GetScreenWidth() - 100,
                BLACK
            );
            // todo: real logic here ...
            DrawText("TWO ARMY MERGE MENU", 200, 200, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER))
            {
                game->campaign_state.overlay_mode = CAMP_OVERLAY_NONE;
            }
        }
        break;

        // Display the units and kraft and tiles etc. of all factions in a table
        case CAMP_OVERLAY_STATISTICS:
        {
            DrawRectangle(
                100,
                100,
                GetScreenHeight() - 100,
                GetScreenWidth() - 100,
                BLACK
            );
            // todo: real logic here ...
            DrawText("STATISTICS-VIEW", 200, 200, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER))
            {
                game->campaign_state.overlay_mode = CAMP_OVERLAY_NONE;
            }
        }
        break;

        case CAMP_OVERLAY_NONE:
        {
            // ...
        }
        break;
    }
}

#endif //CG_DRAW_OVERLAY_H
