//
// Created by mo on 04.05.26.
//

#ifndef CG_AI_BEHAVIOUR_H
#define CG_AI_BEHAVIOUR_H

#include "../types.h"
#include "move_army.h"


void camp__game_logic(Game *game)
{

    // we don't want to progress AI moves and other logic if an overlay is open
    // since AI can attack the player, this effectively stops the AI moves from
    // progressing while the player is attacked -> then the player can decide
    // if he fights a rts battle or does an "autobattle"
    // NOTE: in the first version the player can only do "autobattle"
    if ( game->campaign_state.overlay_mode != CAMP_OVERLAY_NONE)
    {
        return;
    }

    static float next_ai_faction_move_timer = 0.0f;
    static int moves_per_ai_faction_counter = 0;

    if ( next_ai_faction_move_timer > 0 )
    {
        next_ai_faction_move_timer -= GetFrameTime();
    }

    // next round
    if (game->campaign_state.current_active_faction_index == PLAYER_FACTION_INDEX)
    {
        const int end_player_turn = IsKeyPressed(KEY_BACKSPACE);
        if (end_player_turn)
        {
            game->campaign_state.current_active_faction_index = PLAYER_FACTION_INDEX + 1;
            next_ai_faction_move_timer = 1.0f;
        }
    } else // enemy faction turn
    {
        if (next_ai_faction_move_timer <= 0)
        {
            // NOTE: AI move can also be just a move attempt.
            next_ai_faction_move_timer = 1.0f;

            // todo: move army
            {
                // determine what armies to move and to where
                // todo: get all tiles around tile
                // todo: get all 2, 3 tiles around tile
            }

            // todo: create army
            {
                // todo: get all tiles of one faction
            }

            moves_per_ai_faction_counter++;
            if (moves_per_ai_faction_counter >= MAX_MOVES_PER_AI_FACTION)
            {
                // progress to the next enemy faction OR back to player if all factions where processed
                game->campaign_state.current_active_faction_index++;
                if (game->campaign_state.current_active_faction_index >= TOTAL_CAMP_FACTIONS - 1)
                {
                    game->campaign_state.current_active_faction_index = PLAYER_FACTION_INDEX;

                    // reset movement bool
                    {
                        for (int index = 0; index < CAMP_MAP_IN_TILES; index++)
                        {
                            CampTile *tile = &(game->campaign_state.tiles[index]);
                            if (tile->army.alive)
                            {
                                tile->army.can_move_this_turn = 1;
                            }
                        }
                    }

                    // increase kraft for all factions since here is factually a new round
                    {
                        // todo
                    }
                }
            }
        }
    }
}

#endif //CG_AI_BEHAVIOUR_H
