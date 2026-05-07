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

    game->campaign_state.current_active_faction_index = PLAYER_FACTION_INDEX + 1;

    for (int faction_index = 0; faction_index < TOTAL_CAMP_FACTIONS; faction_index++)
    {
        Faction *faction = &(game->campaign_state.factions[faction_index]);
        // these values are needed ibn the ai behaviour logic
        faction->moves_per_ai_faction_counter = 0;
        faction->next_ai_faction_move_timer = 0.0f;
    }



}

#endif
