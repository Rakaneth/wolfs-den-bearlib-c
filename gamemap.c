#include "gamemap.h"
#include "mathutils.h"
#include "new.h"
#include <stdio.h>
#include <string.h>

static Tile tiles[7] = {{"null", 0, 0},
                        {"floor", TA_WALK | TA_SEE, '.'},
                        {"wall", 0, '#'},
                        {"closed door", 0, '+'},
                        {"open door", TA_WALK | TA_SEE, '/'},
                        {"stairs down", TA_WALK | TA_SEE, '>'},
                        {"stairs up", TA_WALK | TA_SEE, '<'}};

static unsigned mapgen_index_rotate(unsigned idx) {
    return (4 * idx) % 255;
}

static int floor_if_has_flag(unsigned flagset, unsigned flag) {
    return (flagset & flag) == flag ? TILE_FLOOR : TILE_WALL;
}

static void mapgen_set_index(GameMap* m, unsigned idx, int x, int y) {
    static char N = 0x01;
    static char NE = 0x02;
    static char E = 0x04;
    static char SE = 0x08;
    static char S = 0x10;
    static char SW = 0x20;
    static char W = 0x40;
    static char NW = 0x80;

    if (idx == 0) {
        for (int xs = -1; xs <= 1; xs++) {
            for (int ys = -1; ys <= 1; ys++) {
                map_set_tile(m, x + xs, y + ys, TILE_FLOOR);
            }
        }
    } else {
        map_set_tile(m, x, y - 1, floor_if_has_flag(idx, N));
        map_set_tile(m, x + 1, y - 1, floor_if_has_flag(idx, NE));
        map_set_tile(m, x + 1, y, floor_if_has_flag(idx, E));
        map_set_tile(m, x + 1, y + 1, floor_if_has_flag(idx, SE));
        map_set_tile(m, x, y + 1, floor_if_has_flag(idx, S));
        map_set_tile(m, x - 1, y + 1, floor_if_has_flag(idx, SW));
        map_set_tile(m, x - 1, y, floor_if_has_flag(idx, W));
        map_set_tile(m, x - 1, y - 1, floor_if_has_flag(idx, NW));
        map_set_tile(m, x, y, TILE_FLOOR);
    }
}

GameMap* map_new(const char* name, int width, int height,
                 const char* wall_color, bool lit) {
    GameMap* new_map;
    size_t tiles_length;
    int x;
    int y;

    tiles_length = width * height;
    new_map = malloc(sizeof(GameMap) + tiles_length * sizeof(TILE_TYPE));

    new_map->width = width;
    new_map->height = height;
    new_map->right_edge = width - 1;
    new_map->bot_edge = height - 1;
    new_map->lit = lit;
    new_map->name = strdup(name);
    new_map->wall_color = strdup(wall_color);
    new_map->tiles_length = tiles_length;
    for (int i = 0; i < tiles_length; i++) {
        x = i % width;
        y = i / width;
        map_set_tile(new_map, x, y, TILE_NULL);
    };

    return new_map;
}

bool map_in_bounds(GameMap* m, int x, int y) {
    return between(x, 0, m->width - 1) && between(y, 0, m->height - 1);
}

Tile map_get_tile(GameMap* m, int x, int y) {
    int t;
    Tile result;
    if (map_in_bounds(m, x, y)) {
        t = m->tiles[m->width * y + x];
        result = tiles[t];
    } else {
        result = tiles[0];
    }
    return result;
}

void map_set_tile(GameMap* m, int x, int y, int t) {
    if (map_in_bounds(m, x, y)) {
        m->tiles[m->width * y + x] = t;
    }
}

bool map_can_walk(GameMap* m, int x, int y) {
    Tile t;

    t = map_get_tile(m, x, y);
    return (t.access & TA_WALK) == TA_WALK;
}

bool map_can_see(GameMap* m, int x, int y) {
    Tile t;

    t = map_get_tile(m, x, y);
    return (t.access & TA_SEE) == TA_SEE;
}

bool map_in_interior(GameMap* m, int x, int y) {
    return between(x, 1, m->right_edge) && between(y, 1, m->bot_edge);
}

void map_destroy(GameMap* m) {
    printf("Destroying %s\n", m->name);
    fflush(stdout);
    free(m->tiles);
    free(m->name);
    free(m->wall_color);
    free(m);
}

void map_debug(GameMap* m) {
    for (int ys = 0; ys < m->height; ys++) {
        for (int xs = 0; xs < m->width; xs++) {
            putchar(map_get_tile(m, xs, ys).glyph);
        }
        putchar('\n');
    }
}

unsigned char map_adj(GameMap* m, Point p) {
    Point cur;
    unsigned char result = 0;
    Tile t;

    for (int i = 0; i < 8; i++) {
        cur = pt_move_direction(p, i);
        t = map_get_tile(m, cur.x, cur.y);
        if (strcmp("floor", t.name) == 0) {
            result |= (1 << i);
        }
    }
    return result;
}
