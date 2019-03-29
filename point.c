#include <stdlib.h>
#include "point.h"

int pt_distance(Point a, Point b)
{
    int x_dist = a.x - b.x;
    int y_dist = a.y - b.y;
    int result = max(x_dist, y_dist);
    return result;
}

int pt_adj(Point a, Point b)
{
    return pt_distance(a, b) == 1;
}

int pt_equal(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

Point pt_translate(Point pt, int dx, int dy)
{
    Point result;
    result.x = pt.x + dx;
    result.y = pt.y + dy;
    return result;
}