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
                            // used to determine where to go
                            CampTilesAroundResult result_with_malloc = get_camp_tiles_around(
                                game,
                                tile,
                                (CampTilesAroundFilter){
                                    .depth = 1,
                                    .only_passable = 1
                                }
                            );
                            if (result_with_malloc.success == 0)
                            {
                                printf(result_with_malloc.reason);
                                exit(EXIT_FAILURE);
                            }

                            // used to determine what to do over all
                            CampTilesAroundResult result_with_malloc_view = get_camp_tiles_around(
                                game,
                                tile,
                                (CampTilesAroundFilter){
                                    .depth = 3,
                                    .only_passable = 1
                                }
                            );
                            if (result_with_malloc_view.success == 0)
                            {
                                printf(result_with_malloc_view.reason);
                                exit(EXIT_FAILURE);
                            }

                            printf("HELLOP11--"); fflush(stdout);
                            /**
                             * If there is another tile in range, that would make sense to move to, since it
                             * is neutral or better to defend.
                             */
                            CampTile *target_tile_move_potential = NULL;

                            /**
                             * If there is an enemy to attack nearby.
                             */
                            CampTile *target_tile_attack_potential = NULL;

                            /**
                             * If there is another of my armies near by that would make sense to merge
                             * with.
                             */
                            CampTile *target_tile_merge_potential = NULL;

                            /**
                             * Enemy armies in the view fields.
                             */
                            int enemy_armies_in_view = 0;

                            /**
                             * Resources in view fields.
                             */
                            int resources_in_view = 0;
                            /**
                             * Cities in view.
                             */
                            int cities_in_view = 0;
                            /**
                              *
                              */
                            int valuable_resources_and_cities_not_owned_in_view = 0;

                            for (
                                int view_result_tile_index = 0;
                                view_result_tile_index <= result_with_malloc_view.amount-1;
                                view_result_tile_index++
                            )
                            {
                                printf("LOLOLOLO; %d\n", view_result_tile_index); fflush(stdout);
                                const CampTile *view_tile = result_with_malloc_view.malloced_tiles[view_result_tile_index];

                                if (
                                    view_tile->terrain_type == CAMP_TERRAIN_TYPE_MOUNTAIN
                                    || view_tile->terrain_type == CAMP_TERRAIN_TYPE_WATER
                                )
                                {
                                    continue;
                                }
                                if (view_tile->terrain_type == CAMP_TERRAIN_TYPE_CITY)
                                {
                                    cities_in_view++;
                                }
                                if (view_tile->terrain_type == CAMP_TERRAIN_TYPE_RESOURCE)
                                {
                                    resources_in_view++;
                                }
                                if ( view_tile->army.alive && view_tile->army.faction != faction)
                                {
                                    enemy_armies_in_view++;
                                }
                            }

                            for (
                                int around_result_tile_index = 0;
                                around_result_tile_index < result_with_malloc.amount;
                                around_result_tile_index++
                            )
                            {
                                printf("FOOOP; %d\n", around_result_tile_index); fflush(stdout);
                                /*const CampTile *maybe_move_to_tile = result_with_malloc.malloced_tiles[
                                    around_result_tile_index];

                                if (tile->owner_faction != maybe_move_to_tile->owner_faction)
                                {
                                    if (maybe_move_to_tile->owner_faction == NULL)
                                    {
                                        // TODO
                                    } else
                                    {
                                        // TODO
                                    }
                                }*/
                            }

                            printf("FOOOPjjjjjjjj;"); fflush(stdout);

                        end_army_movement:
                            printf("tttttt"); fflush(stdout);
                            camp_tiles_around_result_destroy(&result_with_malloc);
                            printf("hhhhhhh"); fflush(stdout);
                            camp_tiles_around_result_destroy(&result_with_malloc_view);


                            // NOTE: if we don't decide to move this arm, we still need to set its can move this turn to 0
                            if (tile->army.can_move_this_turn == 1)
                            {
                                tile->army.can_move_this_turn = 0;
                            }
                        }
                    }
                }
            }
        }
        printf("HELLOP2"); fflush(stdout);
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

        printf("HELLOP3"); fflush(stdout);

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
