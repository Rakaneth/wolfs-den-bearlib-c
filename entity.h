#pragma once
#include "point.h"

enum ENTITY_TYPE {
    ENTITY_CREATURE = 1,
    ENTITY_EQUIP = 1 << 1,
    ENTITY_CONSUMABLE = 1 << 2
};

typedef struct entity_t {
    char* name;
    char* desc;
    char* color;
    int glyph;
    Point pos;
    int type;
    unsigned id;
} Entity;

Entity* entity_new(int glyph, const char* name, const char* desc, int e_type);
void entity_destroy(Entity* e);
void entity_try_move(Entity* e, DIRECTION d);
void entity_debug(Entity* e);
