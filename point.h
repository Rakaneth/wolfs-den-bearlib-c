#pragma once
#include "mathutils.h"

typedef struct point_t
{
    int x;
    int y;
} Point;

int pt_distance(Point a, Point b);
int pt_adj(Point a, Point b);
int pt_equal(Point a, Point b);
Point pt_translate(Point pt, int dx, int dy);
