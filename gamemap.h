#pragma once
#include "point.h"
#include <stdbool.h>
#include <stdlib.h>

typedef enum tiletype_e {
    TILE_NULL,
    TILE_FLOOR,
    TILE_WALL,
    TILE_DOOR_CLOSED,
    TILE_DOOR_OPEN,
    TILE_STAIRS_DOWN,
    TILE_STAIRS_UP
} TILE_TYPE;

typedef enum tileaccess_e { TA_WALK = 1 << 0, TA_SEE = 1 << 1 } TILE_ACCESS;

typedef struct gamemap_t {
    size_t tiles_length;
    int width;
    int height;
    int right_edge;
    int bot_edge;
    bool lit;
    char* name;
    char* wall_color;
    TILE_TYPE tiles[];
} GameMap;

typedef struct tile_t {
    const char* name;
    int access;
    int glyph;
} Tile;

GameMap* map_new(const char* name, int width, int height,
                 const char* wall_color, bool lit);
bool map_in_bounds(GameMap* m, int x, int y);
Tile map_get_tile(GameMap* m, int x, int y);
void map_set_tile(GameMap* m, int x, int y, int t);
bool map_can_walk(GameMap* m, int x, int y);
bool map_can_see(GameMap* m, int x, int y);
bool map_in_interior(GameMap* m, int x, int y);
void map_destroy(GameMap* m);
void map_debug(GameMap* m);
unsigned char map_adj(GameMap* m, Point p);