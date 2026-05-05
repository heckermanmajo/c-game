/*
 TODO-LIST:

 Logistik-System für kraft investment basierend auf cities
 Das macht das AI-erstellen sehr viel einfacher.
 Zudem wenn die ai logistik center mit armeen bauen kann.


- move army and trigger correct overlay
- add log file to camp so we can file log into camp file
- ui should consume the mouse if clicked on

- ZOOM (zoom modes, different displays, zoom-move-factor)
- army movement
- AI Behavior
- army creation (cities)
- kraft getting logic
- get around tiles function
- UI header bar (kraft, round)
- after battle popup view with details, before and after
- null checks, documentation, safety checks
- can move this round indicator
- debugging code auslagern in eine eigen datei; debug prints
 */

/**
 * PARTIAL WAR
 *
 */

#include <stdio.h>
#include <raylib.h>

#include "src/lib/text_button.h"

#include "src/types.h"

#include "src/camp/init.h"
#include "src/camp/move_camera.h"
#include "src/camp/draw_world.h"
#include "src/camp/game_logic.h"
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

    camp__init(&game);

    while (game.is_running)
    {
        if (WindowShouldClose())
        {
            game.is_running = false;
        }

        camp__move_camera(&game);

        camp__user_input_controller(&game);

        camp__game_logic(&game);

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
