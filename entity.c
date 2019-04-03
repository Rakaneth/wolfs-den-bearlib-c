#include "entity.h"
#include "new.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned counter = 0;

Entity* entity_new(int glyph, const char* name, const char* desc, int e_type) {
    Entity* new_entity = NEW(Entity, 1);
    new_entity->name = strdup(name);
    new_entity->desc = strdup(desc);
    new_entity->pos = (Point){0, 0};
    new_entity->glyph = glyph;
    new_entity->type = e_type;
    new_entity->id = ++counter;
    new_entity->color = strdup("white");
    return new_entity;
}

void entity_destroy(Entity* e) {
    printf("Destroying %s\n", e->name);
    fflush(stdout);
    free(e->name);
    e->name = NULL;
    free(e->desc);
    e->desc = NULL;
    free(e->color);
    free(e);
    e = NULL;
}

void entity_try_move(Entity* e, DIRECTION d) {
    e->pos = pt_move_direction(e->pos, d);
}

void entity_debug(Entity* e) {
    printf("%c %s ID: %d\n", e->glyph, e->name, e->id);
}