//
// Created by mo on 03.05.26.
//

#ifndef CG_MOVE_ARMY_H
#define CG_MOVE_ARMY_H

#include "../types.h"

/**
 * Movement of an army can be the following:
 *
 * - just move to own territory
 * - just move to own territory with another army on top (merge dialog/ai auto descision)
 * - just move to neutral territory
 * - just move to enemy territory with another army on top (fight dialog/ai auto battle)
 * - just move to enemy territory
 *
 * In case of a fight or merge where the player is implicated the game needs to go into
 * an "overlay-screen" mode.
 *
 * @param from the start tile (currently selected one for player)
 * @param to target (passable neighbor)
 * @return
 */
void move_army(Game *game, CampTile *from, CampTile *to);


#endif //CG_MOVE_ARMY_H

#ifdef IMPLEMENTATION

void move_army(Game *game, CampTile *from, CampTile *to)
{
    if (from == NULL)
    {
        printf("given from is null\n");
        exit(EXIT_FAILURE);
    }

    if (to == NULL)
    {
        printf("given to is null\n");
        exit(EXIT_FAILURE);
    }

    if (from == to)
    {
        printf("from and to are equal\n");
        exit(EXIT_FAILURE);
    }

    if (to->terrain_type == CAMP_TERRAIN_TYPE_MOUNTAIN || to->terrain_type == CAMP_TERRAIN_TYPE_WATER)
    {
        // this is a valid call, but we just don't do anything here ...
        return;
    }

    if (from->army.alive == 0 || from->army.can_move_this_turn == 0)
    {
        // can happen -> no effect
        return;
    }

    if (from->owner_faction == to->owner_faction)
    {
        if (to->army.alive == 0)
        {
            to->army.faction = from->army.faction;
            to->army.kraft = from->army.kraft;
            to->army.alive = from->army.alive;
            to->army.tile_i_am_on = to;
            to->army.can_move_this_turn = 0;

            from->army.alive = 0;
            from->army.kraft = 0;
            from->army.tile_i_am_on = 0;
            from->army.can_move_this_turn = 0;
            from->army.faction = NULL;
        } else
        {
            // merge army
            to->owner_faction = from->army.faction;

            to->army.faction = from->army.faction;
            to->army.kraft += from->army.kraft;
            to->army.alive = from->army.alive;
            to->army.tile_i_am_on = to;
            to->army.can_move_this_turn = 0;

            from->army.alive = 0;
            from->army.kraft = 0;
            from->army.tile_i_am_on = NULL;
            from->army.can_move_this_turn = 0;
            from->army.faction = NULL;
        }
    } else
    {
        if (to->owner_faction == NULL)
        {
            to->owner_faction = from->army.faction;

            to->army.faction = from->army.faction;
            to->army.kraft = from->army.kraft;
            to->army.alive = from->army.alive;
            to->army.tile_i_am_on = to;
            to->army.can_move_this_turn = 0;

            from->army.alive = 0;
            from->army.kraft = 0;
            from->army.tile_i_am_on = NULL;
            from->army.can_move_this_turn = 0;
            from->army.faction = NULL;
            // todo so sth more???
        } else // enemy
        {
            if (to->army.alive == 0)
            {
                // conquer the enemy tile
                to->owner_faction = from->army.faction;

                to->army.faction = from->army.faction;
                to->army.kraft = from->army.kraft;
                to->army.alive = from->army.alive;
                to->army.tile_i_am_on = to;
                to->army.can_move_this_turn = 0;

                // destroy the logistics hub if we conquer the enemy
                if ( to->terrain_type == CAMP_TERRAIN_TYPE_LOGISTICS_HUB)
                {
                    to->terrain_type = CAMP_TERRAIN_TYPE_GRASS;
                }

                from->army.alive = 0;
                from->army.kraft = 0;
                from->army.tile_i_am_on = NULL;
                from->army.can_move_this_turn = 0;
                from->army.faction = NULL;
                // todo: conquer non army enemy territory
                // todo: maybe worsen relationship
                // todo: later here is a place to open an overlay, if this would break a peace
            } else
            {
                if (from->owner_faction->is_player == 0 && to->owner_faction->is_player == 0)
                {
                    // todo: auto resolve and apply changes
                    // cannot happen currently -> v01 -> hat nur zwei factions
                } else
                {
                    // todo: trigger overlay if to or from is player
                    // fort now also auto resolve
                    int attack_force = from->army.kraft;
                    int defend_force = to->army.kraft;
                    from->army.kraft -= (int)((float)defend_force * ((float)GetRandomValue(80, 120) / 100.0));
                    to->army.kraft -= (int)((float)attack_force * ((float)GetRandomValue(80, 120) / 100.0));

                    if (from->army.kraft <= 0)
                    {
                        from->army.alive = 0;
                    }

                    if (to->army.kraft <= 0)
                    {
                        to->army.alive = 0;
                    }
                }
            }
        }
    }
}

#endif
