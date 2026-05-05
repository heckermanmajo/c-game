//
// Created by mo on 05.05.26.
//

#ifndef CG_PROGRESS_TO_NEXT_ROUND_H
#define CG_PROGRESS_TO_NEXT_ROUND_H

void camp__progress_to_next_round(Game* game);

#endif //CG_PROGRESS_TO_NEXT_ROUND_H

#ifndef IMPLEMENTATION

void camp__progress_to_next_round(Game* game)
{
    game->campaign_state.current_round++;

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

#endif
