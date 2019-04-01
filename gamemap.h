#pragma once
#include <stdlib.h>

enum TILE_TYPE
{
    TILE_NULL,
    TILE_FLOOR,
    TILE_WALL,
    TILE_DOOR_CLOSED,
    TILE_DOOR_OPEN,
    TILE_STAIRS_DOWN,
    TILE_STAIRS_UP
};

enum TILE_ACCESS
{
    TA_WALK = 1 << 1,
    TA_SEE = 1 << 2
};

typedef struct gamemap_t
{
    size_t tiles_length;
    int width;
    int height;
    int *tiles;
    int lit;
    char *name;
} GameMap;

typedef struct tile_t
{
    const char *name;
    int access;
    int glyph;
} Tile;

GameMap *map_new(const char *name, int width, int height, int lit);
int map_in_bounds(GameMap *m, int x, int y);
Tile map_get_tile(GameMap *m, int x, int y);
void map_set_tile(GameMap *m, int x, int y, int t);
int map_can_walk(GameMap *m, int x, int y);
int map_can_see(GameMap *m, int x, int y);
void map_destroy(GameMap *m);
void map_debug(GameMap *m);