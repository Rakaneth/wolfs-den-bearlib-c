#include "point.h"
#include <stdlib.h>

static int DIRS[8][2] = {{0, -1}, {1, -1}, {1, 0},  {1, 1},
                         {0, 1},  {-1, 1}, {-1, 0}, {-1, -1}};

int pt_distance(Point a, Point b) {
    int x_dist = a.x - b.x;
    int y_dist = a.y - b.y;
    int result = max(x_dist, y_dist);
    return result;
}

bool pt_adj(Point a, Point b) {
    return pt_distance(a, b) == 1;
}

bool pt_equal(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}

Point pt_translate(Point pt, int dx, int dy) {
    Point result;
    result.x = pt.x + dx;
    result.y = pt.y + dy;
    return result;
}

Point pt_move_direction(Point pt, DIRECTION d) {
    int offset[2];
    offset[0] = DIRS[d][0];
    offset[1] = DIRS[d][1];
    return pt_translate(pt, offset[0], offset[1]);
}