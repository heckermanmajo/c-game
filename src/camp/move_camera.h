#include "../types.h"

// move camera
void camp__move_camera(
    Game *game
)
{

    if (game->campaign_state.overlay_mode != CAMP_OVERLAY_NONE)
    {
        return;
    }

    // zoom
    {
        const float moved = GetMouseWheelMove();
        game->campaign_state.camera.zoom += moved / 5;
        if (game->campaign_state.camera.zoom < 0.35f)
        {
            game->campaign_state.camera.zoom = 0.35f;
        }
        if (game->campaign_state.camera.zoom > 4.00f)
        {
            game->campaign_state.camera.zoom = 4.00f;
        }

        if (game->campaign_state.camera.zoom < 0.50f)
        {
            game->campaign_state.zoom_factor = ZOOM_FACTOR_VERY_FAR;
        } else if (game->campaign_state.camera.zoom < 0.7f)
        {
            game->campaign_state.zoom_factor = ZOOM_FACTOR_FAR;
        } else if (game->campaign_state.camera.zoom < 1.3f)
        {
            game->campaign_state.zoom_factor = ZOOM_FACTOR_DEFAULT;
        } else if (game->campaign_state.camera.zoom < 1.7f)
        {
            game->campaign_state.zoom_factor = ZOOM_FACTOR_NEAR;
        } else
        {
            game->campaign_state.zoom_factor = ZOOM_FACTOR_VERY_NEAR;
        }
    }

    const float wasd_camera_move_speed = 10.0f;
    if (IsKeyDown(KEY_W))
    {
        game->campaign_state.camera.target.y -= wasd_camera_move_speed;
    }
    if (IsKeyDown(KEY_S))
    {
        game->campaign_state.camera.target.y += wasd_camera_move_speed;
    }
    if (IsKeyDown(KEY_A))
    {
        game->campaign_state.camera.target.x -= wasd_camera_move_speed;
    }
    if (IsKeyDown(KEY_D))
    {
        game->campaign_state.camera.target.x += wasd_camera_move_speed;
    }

    // drag with mouse
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        const Vector2 mouseDelta = GetMouseDelta();
        game->campaign_state.camera.offset.x += mouseDelta.x;
        game->campaign_state.camera.offset.y += mouseDelta.y;
    }

}
