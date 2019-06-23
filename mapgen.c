#include "mapgen.h"
#include "loadlua.h"
#include "point.h"
#include "randomgen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rect_t {
    int x;
    int y;
    int w;
    int h;
    int x2;
    int y2;
} Rect;

static Rect rect_new(int x, int y, int w, int h) {
    Rect new_rect;
    new_rect.x = x;
    new_rect.y = y;
    new_rect.w = w;
    new_rect.h = h;
    new_rect.x2 = x + w - 1;
    new_rect.y2 = y + h - 1;
    return new_rect;
}

static bool rect_intersect(Rect a, Rect b) {
    bool result = true;

    if (a.x2 < b.x || b.x2 < a.x) {
        result = false;
    }

    if (a.y2 < b.y || b.y2 < a.y) {
        result = false;
    }

    return result;
}

static Point center(Rect r) {
    int x = (r.x2 + r.x) / 2;
    int y = (r.y2 + r.y) / 2;
    return (Point){x, y};
}

static void carve_rect(GameMap* m, Rect r) {
    for (int xs = r.x + 1; xs < r.x2; xs++) {
        for (int ys = r.y + 1; ys < r.y2; ys++) {
            map_set_tile(m, xs, ys, TILE_FLOOR);
        }
    }
}

static void all_walls(GameMap* m) {
    int x, y;
    for (int i = 0; i < m->tiles_length; i++) {
        x = i % m->width;
        y = i / m->width;
        map_set_tile(m, x, y, TILE_WALL);
    }
}

static void wall_edges(GameMap* m) {
    for (int x = 0; x < m->width; x++) {
        map_set_tile(m, x, 0, TILE_WALL);
        map_set_tile(m, x, m->bot_edge, TILE_WALL);
    }

    for (int y = 0; y < m->height; y++) {
        map_set_tile(m, 0, y, TILE_WALL);
        map_set_tile(m, m->right_edge, y, TILE_WALL);
    }
}

static void place_rooms(GameMap* m) {
    static const int MAX_ROOMS = 200;
    static const int MIN_SIZE = 5;
    static const int MAX_SIZE = 10;
    Rect rooms[200];
    Rect new_rect;
    int nb_rooms = 0;
    bool clear;
    int x, y, w, h;

    for (int tries = 0; tries < MAX_ROOMS; tries++) {
        clear = true;
        x = get_rand_int(1, m->width - 1 - MAX_SIZE);
        y = get_rand_int(1, m->height - 1 - MAX_SIZE);
        w = get_rand_int(MIN_SIZE, MAX_SIZE);
        h = get_rand_int(MIN_SIZE, MAX_SIZE);
        printf("Attempt #%d: placing %dx%d room at (%d, %d): ", tries + 1, w, h,
               x, y);
        new_rect = rect_new(x, y, w, h);

        if (nb_rooms > 0) {
            for (int i = 0; i < nb_rooms; i++) {
                if (rect_intersect(rooms[i], new_rect)) {
                    clear = false;
                    break;
                }
            }
        }

        if (clear) {
            rooms[nb_rooms++] = new_rect;
            carve_rect(m, new_rect);
            printf("success\n");
        } else {
            printf("failure\n");
        }

        fflush(stdout);
    }
}

typedef struct digger_t {
    Point pos;
    int duration;
    DIRECTION dir;
} Digger;

static void digger_carve(GameMap* m, Digger d) {
    while (map_in_interior(m, d.pos.x, d.pos.y) && d.duration > 0) {
        map_set_tile(m, d.pos.x, d.pos.y, TILE_FLOOR);
        d.pos = pt_move_direction(d.pos, d.dir);
        d.duration--;
    }
}

static GameMap* generate_digger(const char* name, int width, int height,
                                const char* wall_color, bool lit) {
    GameMap* new_map;
    Digger d;
    unsigned int turn_check;
    int num_tiles = 0;
    int max_tiles;

    new_map = map_new(name, width, height, wall_color, lit);
    all_walls(new_map);
    max_tiles = new_map->tiles_length / 5;

    d.pos.x = get_rand_int(1, new_map->right_edge);
    d.pos.y = get_rand_int(1, new_map->bot_edge);
    d.duration = get_rand_int(2, 20);
    d.dir = get_rand_int(0, 3) * 2;

    wall_edges(new_map);

    return new_map;
}

static GameMap* generate_dungeon(const char* name, int width, int height,
                                 const char* wall_color, bool lit) {
    GameMap* new_map;

    new_map = map_new(name, width, height, wall_color, lit);
    all_walls(new_map);
    place_rooms(new_map);
    return new_map;
}

GameMap* generate_map(const char* build_id) {
    lua_State* L = load_lua_file("lua/maps.lua");
    int w, h;
    const char* map_type;
    const char* map_name;
    const char* map_color;
    GameMap* new_map;

    lua_getglobal(L, "mapdata");
    lua_pushstring(L, "mine");
    dump_stack(L);
    lua_gettable(L, -2);
    w = table_get_int(L, "width");
    h = table_get_int(L, "height");
    map_type = table_get_string(L, "type");
    map_color = table_get_string(L, "wall_color");
    map_name = table_get_string(L, "name");

    if (map_type == NULL) {
        map_type = "dungeon";
    }

    if (map_color == NULL) {
        map_color = "127,101,95";
    }

    if (map_name == NULL) {
        map_name = "No map name";
    }

    if (strcmp(map_type, "dungeon") == 0) {
        new_map = generate_dungeon(map_name, w, h, map_color, true);
    } else if (strcmp(map_type, "digger") == 0) {
        new_map = generate_digger(map_name, w, h, map_color, true);
    } else {
        fprintf(stderr, "Dungeon type %s is not yet implemented", map_type);
        fflush(stderr);
        exit(1);
    }

    return new_map;
}