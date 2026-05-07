/*
 TODO-LIST:


- AI behaviour

- ui should consume the mouse if clicked on

- after battle popup view with details, before and after
- null checks, documentation, safety checks
- can move this round indicator
 */

/**
 * PARTIAL WAR
 *
 * Game 1: Campaign map in simple, itch io
 * Game 2: Campaign map in nice, itch io
 * Game 3: Campaign map in nice with simple battles
 * Game 4: Campaign map in nice with nice battles
 * Game 5: same in multiple mods; landsknecht, modern, future, Medival
 * Game 6: Heros
 * Game 7: building, resources; EE battle-modes and real time battles mixed; mages
 *
 */

#include <stdio.h>
#include <raylib.h>

#include "src/lib/text_button.h"

#include "src/types.h"

#include "src/camp/init.h"
#include "src/camp/move_camera.h"
#include "src/camp/draw_world.h"
#include "src/camp/camp_ai_behaviour.h"
#include "src/camp/debug_printing.h"
#include "src/camp/draw_ui.h"
#include "src/camp/draw_overlay.h"
#include "src/camp/user_input_controller.h"
#include "src/camp/get_distance_between_tiles.h"


#define IMPLEMENTATION
#include "src/camp/get_camp_tiles_around.h"
#include "src/camp/move_army.h"
#include "src/camp/progress_to_next_round.h"
#include "src/camp/get_distance_between_tiles.h"
#undef IMPLEMENTATION


int main()
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(
        1900,
        1200,
        "Game"
    );

    ToggleFullscreen();

    SetTargetFPS(45);

    Game game = {0};
    game.is_running = true;

    camp__init(&game);

    while (game.is_running)
    {
        if (WindowShouldClose())
        {
            game.is_running = false;
        }

        camp__move_camera(&game);

        camp__user_input_controller(&game);

        camp__ai_behaviour(&game);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(game.campaign_state.camera);

        camp__draw_world(&game);

        EndMode2D();

        camp__draw_ui(&game);

        camp__draw_overlay(&game);

        camp__debug_print(&game);

        EndDrawing();
    } // end main game loop

    // cleanup
    {
        camp_tiles_around_result_destroy(&game.campaign_state.camp_tiles_around);
    }
}
