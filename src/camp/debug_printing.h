//
// Created by mo on 05.05.26.
//

#ifndef CG_DEBUG_PRINTING_H
#define CG_DEBUG_PRINTING_H

#include "../types.h"

void camp__debug_print(Game* game)
{
    int y = 70;
    {
        char *mode = "NONE";
        switch (game->campaign_state.debug_mode)
        {
            case DEBUG_MODE_GET_CAMP_TILES_AROUND: mode = "GET_CAMP_TILES_AROUND";
                break;
            case DEBUG_MODE_NONE: mode = "NONE";
                break;
        }

        DrawText(
            TextFormat("debugmode: %s (f10) ", mode),
            10,
            y,
            20,
            BLUE
        );
    }

    {
        if (IsKeyPressed(KEY_F11))
        {
            switch (game->campaign_state.debug_mode)
            {
                case DEBUG_MODE_GET_CAMP_TILES_AROUND:
                    camp_tiles_around_result_destroy(&game->campaign_state.camp_tiles_around);
                    game->campaign_state.debug_mode = DEBUG_MODE_NONE;
                    break;
                case DEBUG_MODE_NONE: game->campaign_state.debug_mode = DEBUG_MODE_GET_CAMP_TILES_AROUND;
                    break;
            }
        }
    }

    {
        char *zoom_level = "NONE";
        switch (game->campaign_state.zoom_factor)
        {
            case ZOOM_FACTOR_VERY_NEAR: zoom_level = "VERY_NEAR";
                break;
            case ZOOM_FACTOR_NEAR: zoom_level = "NEAR";
                break;
            case ZOOM_FACTOR_DEFAULT: zoom_level = "ZOOM_FACTOR_DEFAULT";
                break;
            case ZOOM_FACTOR_FAR: zoom_level = "FAR";
                break;
            case ZOOM_FACTOR_VERY_FAR: zoom_level = "VERY_FAR";
                break;
        }
        y += 30;
        DrawText(
            TextFormat(
                "Zoom %s ( %f )", zoom_level, game->campaign_state.camera.zoom
            ),
            10,
            y,
            20,
            BLUE
        );
    }
}


#endif //CG_DEBUG_PRINTING_H
