#ifndef CG_TYPES_H
#define CG_TYPES_H

#include <raylib.h>
#include "camp/get_camp_tiles_around.h"

enum CAMP_TERRAIN_TYPE
{
    CAMP_TERRAIN_TYPE_GRASS,
    CAMP_TERRAIN_TYPE_WATER,
    CAMP_TERRAIN_TYPE_CITY,
    CAMP_TERRAIN_TYPE_LOGISTICS_HUB,
    CAMP_TERRAIN_TYPE_RESOURCE,
    CAMP_TERRAIN_TYPE_MOUNTAIN,
};

enum ZOOM_FACTOR
{
    ZOOM_FACTOR_VERY_NEAR,
    ZOOM_FACTOR_NEAR,
    ZOOM_FACTOR_DEFAULT,
    ZOOM_FACTOR_FAR,
    ZOOM_FACTOR_VERY_FAR,
};

/**
 * We use this debug mode to toggle different ui-tools for visually debugging
 * the code.
 */
enum DEBUG_MODE
{
    DEBUG_MODE_NONE,
    DEBUG_MODE_GET_CAMP_TILES_AROUND
};

/**
 * This enum determines what and if we display an overlay;
 * During an overlay, we cannot interact with the map, but only
 * with the overlay.
 */
enum OVERLAY_CAMP_MODE
{
    CAMP_OVERLAY_NONE,
    CAMP_OVERLAY_MERGE,
    CAMP_OVERLAY_BATTLE,
    CAMP_OVERLAY_BATTLE_RESULT,
    CAMP_OVERLAY_MENU,
    CAMP_OVERLAY_STATISTICS
};

#define CAMP_TILE_SIZE_IN_PIXELS 128
#define CAMP_MAP_SIZE 10
#define CAMP_MAP_IN_TILES  CAMP_MAP_SIZE * CAMP_MAP_SIZE
#define CAMP_SIZE_IN_PIXELS CAMP_TILE_SIZE_IN_PIXELS * CAMP_MAP_SIZE
#define TOTAL_CAMP_FACTIONS 2
#define PLAYER_FACTION_INDEX 0
#define MAX_MOVES_PER_AI_FACTION 10

typedef struct Faction Faction;
typedef struct Army Army;
typedef struct CampaignState CampaignState;
typedef struct CampTile CampTile;
typedef struct Game Game;

struct Faction
{
    const char* name;
    int is_player;
    Color color;
    int kraft;
};

struct Army
{
    int alive;
    Faction* faction;
    CampTile* tile_i_am_on;
    int kraft;
    int can_move_this_turn;
};

struct CampTile
{
    int x_index, y_index;
    int x_world, y_world;
    enum CAMP_TERRAIN_TYPE terrain_type;
    Faction* owner_faction;
    Army army;
    int kraft_per_round;
};

struct CampaignState
{
    CampTile tiles[CAMP_MAP_IN_TILES];
    Faction factions[TOTAL_CAMP_FACTIONS];
    Camera2D camera;
    CampTile* currently_selected_tile;
    CampTilesAroundResult camp_tiles_around;
    int current_round;
    int current_active_faction_index;
    enum DEBUG_MODE debug_mode;
    enum ZOOM_FACTOR zoom_factor;
    enum OVERLAY_CAMP_MODE overlay_mode;
    struct MergeOverlayState
    {
        CampTile* from;
        CampTile* to;
    } merge_overlay_state;

    struct BattleOverlayState
    {
        CampTile* from;
        CampTile* to;
    } battle_overlay_state;

};

struct Game
{
    int is_running;
    CampaignState campaign_state;
};

#endif //CG_TYPES_H
