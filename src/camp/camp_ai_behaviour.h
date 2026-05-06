//
// Created by mo on 04.05.26.
//

#ifndef CG_AI_BEHAVIOUR_H
#define CG_AI_BEHAVIOUR_H

#include "../types.h"
#include "move_army.h"


void camp__ai_behaviour(Game *game)
{
    // we don't want to progress AI moves and other logic if an overlay is open
    // since AI can attack the player, this effectively stops the AI moves from
    // progressing while the player is attacked -> then the player can decide
    // if he fights a rts battle or does an "autobattle"
    // NOTE: in the first version the player can only do "autobattle"
    if (game->campaign_state.overlay_mode != CAMP_OVERLAY_NONE)
    {
        return;
    }

    if (game->campaign_state.current_active_faction_index == PLAYER_FACTION_INDEX)
    {
        return;
    }

    Faction *faction = &(game->campaign_state.factions[game->campaign_state.current_active_faction_index]);
    if (faction->next_ai_faction_move_timer <= 0)
    {
        // NOTE: AI move can also be just a move attempt.
        faction->next_ai_faction_move_timer = 1.0f;

        // move army
        {
            for (int index = 0; index < CAMP_MAP_IN_TILES; index++)
            {
                CampTile *tile = &(game->campaign_state.tiles[index]);
                if (tile->owner_faction == faction)
                {
                    if (tile->army.alive)
                    {
                        if (tile->army.can_move_this_turn == 1)
                        {
                            // todo: determine if can need to move
                            // determine what armies to move and to where
                            // high value targets: cities; dan resources, dann unbestezte logistik zentren
                            // todo: get all tiles around tile
                            // todo: get all 2, 3 tiles around tile
                            // TODO: IF ONE WAS MOVED -> reset the timer and then continue
                        }
                    }
                }
            }
        }

        // create army/increae army
        // todo: check if we have enough kraft, else jump this part
        {
            for (int index = 0; index < CAMP_MAP_IN_TILES; index++)
            {
                CampTile *tile = &(game->campaign_state.tiles[index]);
                if (tile->owner_faction == faction)
                {
                    if (tile->army.alive)
                    {
                        if (tile->terrain_type == CAMP_TERRAIN_TYPE_CITY)
                        {
                            // todo: determine if we want to increase an existing army
                            //       or if we want to create a new one
                            //       if one is done, return
                        }
                    }
                }
            }
        }

        faction->moves_per_ai_faction_counter++;
        if (faction->moves_per_ai_faction_counter >= MAX_MOVES_PER_AI_FACTION)
        {
            // progress to the next enemy faction OR back to player if all factions where processed
            game->campaign_state.current_active_faction_index++;
            if (game->campaign_state.current_active_faction_index >= TOTAL_CAMP_FACTIONS - 1)
            {
                game->campaign_state.current_active_faction_index = PLAYER_FACTION_INDEX;

                for (int index = 0; index < CAMP_MAP_IN_TILES; index++)
                {
                    CampTile *tile = &(game->campaign_state.tiles[index]);
                    if (tile->owner_faction == NULL)
                    {
                        continue;
                    }

                    switch (tile->terrain_type)
                    {
                        case CAMP_TERRAIN_TYPE_CITY: tile->owner_faction->kraft += 5;
                            break;
                        case CAMP_TERRAIN_TYPE_RESOURCE: tile->owner_faction->kraft += 10;
                            break;
                        case CAMP_TERRAIN_TYPE_GRASS: tile->owner_faction->kraft += 1;
                            break;
                        default: /* pass */ break;
                    }

                    if (tile->army.alive)
                    {
                        tile->army.can_move_this_turn = 1;
                    }
                }
            }
        }
    }
}

#endif //CG_AI_BEHAVIOUR_H
