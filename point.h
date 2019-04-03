#pragma once
#include "mathutils.h"

typedef struct point_t {
    int x;
    int y;
} Point;

typedef enum direction_e {
    DIR_N,
    DIR_NE,
    DIR_E,
    DIR_SE,
    DIR_S,
    DIR_SW,
    DIR_W,
    DIR_NW
} DIRECTION;

int pt_distance(Point a, Point b);
bool pt_adj(Point a, Point b);
bool pt_equal(Point a, Point b);
Point pt_translate(Point pt, int dx, int dy);
Point pt_move_direction(Point pt, DIRECTION d);
