#include "gamemap.h"
#include "mathutils.h"
#include <string.h>
#include <stdio.h>

static Tile tiles[7] = {
    {"null", 0, 0},
    {"floor", TA_WALK | TA_SEE, '.'},
    {"wall", 0, '#'},
    {"closed door", 0, '+'},
    {"open door", TA_WALK | TA_SEE, '/'},
    {"stairs down", TA_WALK | TA_SEE, '>'},
    {"stairs up", TA_WALK | TA_SEE, '<'}};

GameMap *map_new(const char *name, int width, int height, int lit)
{
    GameMap *new_map;
    int *new_tiles;
    size_t tiles_length;
    int x;
    int y;

    tiles_length = width * height;
    new_tiles = (int *)calloc(tiles_length, sizeof(int));
    new_map = (GameMap *)calloc(sizeof(GameMap), 1);

    new_map->width = width;
    new_map->height = height;
    new_map->lit = lit;
    new_map->tiles = new_tiles;
    new_map->name = strdup(name);
    new_map->tiles_length = tiles_length;
    for (int i = 0; i < new_tiles; i++)
    {
        x = i % width;
        y = i / width;
        map_set_tile(new_map, x, y, TILE_FLOOR);
    }

    return new_map;
}

int map_in_bounds(GameMap *m, int x, int y)
{
    return between(x, 0, m->width - 1) && between(y, 0, m->height - 1);
}

Tile map_get_tile(GameMap *m, int x, int y)
{
    int t;
    Tile result;
    if (map_in_bounds(m, x, y))
    {
        t = m->tiles[m->width * y + x];
        result = tiles[t];
    }
    else
    {
        result = tiles[0];
    }
    return result;
}

void map_set_tile(GameMap *m, int x, int y, int t)
{
    if (map_in_bounds(m, x, y))
    {
        m->tiles[m->width * y + x] = t;
    }
}

int map_can_walk(GameMap *m, int x, int y)
{
    Tile t;

    t = map_get_tile(m, x, y);
    return (t.access & TA_WALK) == TA_WALK;
}

int map_can_see(GameMap *m, int x, int y)
{
    Tile t;

    t = map_get_tile(m, x, y);
    return (t.access & TA_SEE) == TA_SEE;
}

void map_destroy(GameMap *m)
{
    printf("Destroying %s\n", m->name);
    fflush(stdout);
    free(m->tiles);
    free(m->name);
    free(m);
}

void map_debug(GameMap *m)
{
    for (int ys = 0; ys < m->height; ys++)
    {
        for (int xs = 0; xs < m->width; xs++)
        {
            putchar(map_get_tile(m, xs, ys).glyph);
        }
        putchar('\n');
    }
}
