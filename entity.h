#pragma once
#include "point.h"

typedef struct entity_t
{
    char *name;
    char *desc;
    int glyph;
    Point pos;
} Entity;

Entity *entity_new(int glyph, const char *name, const char *desc);
void entity_destroy(Entity *e);
void try_move(Entity *e, int dx, int dy);