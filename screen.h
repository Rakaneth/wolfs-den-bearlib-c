#pragma once

#define SCREEN_W 100
#define SCREEN_H 40
#define MAP_X 0
#define MAP_Y 0
#define MAP_W 60
#define MAP_H 30
#define STAT_X 60
#define STAT_Y 0
#define STAT_W 40
#define STAT_H 30
#define MSG_X 0
#define MSG_Y 30
#define MSG_W 30
#define MSG_H 10
#define SKL_X 30
#define SKL_Y 30
#define SKL_W 30
#define SKL_H 10
#define INFO_X 60
#define INFO_Y 30
#define INFO_W 40
#define INFO_H 10

typedef struct panel_t
{
    int x;
    int y;
    int w;
    int h;
    char *caption;
} Panel;

typedef struct screen_t
{
    char *name;
    void (*render)(void);
    void (*enter)(struct screen_t *self);
    void (*exit)(struct screen_t *self);
    void (*handle)(int key);
} Screen;

typedef struct screen_stack_node_t
{
    Screen *screen;
    struct screen_stack_node_t *next;
} ScreenNode;

Screen *title_screen_new(void);
Screen *main_screen_new(void);

void base_screen_enter(Screen *self);
void base_screen_exit(Screen *self);
void title_screen_render(void);
void title_screen_handle(int key);
void main_screen_enter(Screen *self);
void main_screen_exit(Screen *self);
void main_screen_render(void);
void main_screen_handle(int key);
int screen_stack_pop(ScreenNode *first);
void screen_stack_push(Screen *s, ScreenNode *first);
ScreenNode *screen_stack_peek(ScreenNode *first);
void screen_stack_destroy(ScreenNode *first);
ScreenNode *screen_stack_new(Screen *s);
void screen_node_destroy(ScreenNode *goner);
void screen_stack_debug(ScreenNode *first);
void screen_stack_render(ScreenNode *first);
Panel *panel_new(int x, int y, int w, int h, const char *caption);

ScreenNode *screen_stack;
