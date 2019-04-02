#include "mapgen.h"
#include "point.h"
#include "randomgen.h"
#include <stdio.h>

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

static carve_rect(GameMap* m, Rect r) {
    for (int xs = r.x; xs <= r.x2; xs++) {
        for (int ys = r.y; ys <= r.y2; ys++) {
            map_set_tile(m, xs, ys, TILE_FLOOR);
        }
    }
}

static place_rooms(GameMap* m) {
    static const int MAX_ROOMS = 50;
    static const int MIN_SIZE = 5;
    static const int MAX_SIZE = 10;
    Rect rooms[50];
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

GameMap* generate_dungeon(int width, int height, char* name, bool lit) {
    GameMap* new_map;

    new_map = map_new(name, width, height, lit);
    place_rooms(new_map);
    return new_map;
}