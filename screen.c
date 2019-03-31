#include "screen.h"
#include "stdio.h"
#include "BearLibTerminal.h"
#include <string.h>
#include "entity.h"
#include "gamemap.h"

static Panel *msg_p;
static Panel *map_p;
static Panel *stat_p;
static Panel *info_p;
static Panel *skills_p;
static Entity *test_e;
static GameMap *test_map;

static void handle_alloc_error(const char *name)
{
    fprintf(stderr, "Failed to allocate new %s screen", name);
    fflush(stderr);
    exit(1);
}

static void write_center(const char *text, int y)
{
    int w = terminal_measure(text).width;
    if (w > SCREEN_W)
    {
        fprintf(stderr, "Line %s is too long", text);
        fflush(stderr);
        exit(1);
    }
    terminal_print((SCREEN_W - w) / 2, y, text);
}

static dimensions_t panel_print(Panel *panel, int x, int y, const char *text)
{
    return terminal_print(x + panel->x, y + panel->y, text);
}

void panel_put(Panel *panel, int x, int y, int code)
{
    terminal_put(x + panel->x, y + panel->y, code);
}

static void border(Panel *panel)
{
    static int UL = 0x2554;
    static int UR = 0x2557;
    static int LL = 0x255A;
    static int LR = 0x255D;
    static int HORZ = 0x2550;
    static int VERT = 0x2551;
    int x2 = panel->w - 1;
    int y2 = panel->h - 1;

    for (int xs = 1; xs < x2; xs++)
    {
        panel_put(panel, xs, 0, HORZ);
        panel_put(panel, xs, y2, HORZ);
    }

    for (int ys = 1; ys < y2; ys++)
    {
        panel_put(panel, 0, ys, VERT);
        panel_put(panel, x2, ys, VERT);
    }

    panel_put(panel, 0, 0, UL);
    panel_put(panel, x2, 0, UR);
    panel_put(panel, 0, y2, LL);
    panel_put(panel, x2, y2, LR);

    if (panel->caption != NULL)
    {
        panel_print(panel, 1, 0, panel->caption);
    }
}

static void panel_destroy(Panel *p)
{
    printf("Removing panel");
    printf((p->caption == NULL ? "\n" : " with caption %s\n"), p->caption);
    fflush(stdout);
    free(p->caption);
    p->caption = NULL;
    free(p);
}

Screen *title_screen_new(void)
{
    Screen *s = malloc(sizeof(Screen));
    if (!s)
    {
        handle_alloc_error("title");
    }
    s->enter = base_screen_enter;
    s->exit = base_screen_exit;
    s->handle = title_screen_handle;
    s->name = strdup("title");
    s->render = title_screen_render;
    return s;
}

Screen *main_screen_new(void)
{
    Screen *s = malloc(sizeof(Screen));
    if (!s)
    {
        handle_alloc_error("title");
    }
    s->enter = main_screen_enter;
    s->exit = main_screen_exit;
    s->handle = main_screen_handle;
    s->name = strdup("main");
    s->render = main_screen_render;

    return s;
}

void base_screen_enter(Screen *self)
{
    fprintf(stdout, "Entered %s screen\n", self->name);
    fflush(stdout);
}

void base_screen_exit(Screen *self)
{
    fprintf(stdout, "Exited %s screen\n", self->name);
    fflush(stdout);
}

void title_screen_render()
{
    write_center("Wolf's Den 2: C Edition", 15);
    write_center("by Rakaneth", 16);
    write_center("Press any key to start", 18);
}

void title_screen_handle(int key)
{
    screen_stack_pop(screen_stack);
    screen_stack = screen_stack_new(main_screen_new());
}

void main_screen_enter(Screen *self)
{
    //TODO: chargen
    msg_p = panel_new(MSG_X, MSG_Y, MSG_W, MSG_H, "Messages");
    map_p = panel_new(MAP_X, MAP_Y, MAP_W, MAP_H, NULL);
    stat_p = panel_new(STAT_X, STAT_Y, STAT_W, STAT_H, "Stats");
    info_p = panel_new(INFO_X, INFO_Y, INFO_W, INFO_H, "Info");
    skills_p = panel_new(SKL_X, SKL_Y, SKL_W, SKL_H, "Skills");
    test_e = entity_new('@', "Player", "The player!", ENTITY_CREATURE);
    test_map = map_new("Test", 100, 100, 1);
    map_set_tile(test_map, 12, 34, TILE_DOOR_CLOSED);
    map_debug(test_map);
}

void main_screen_exit(Screen *self)
{
    panel_destroy(msg_p);
    msg_p = NULL;
    panel_destroy(map_p);
    map_p = NULL;
    panel_destroy(skills_p);
    skills_p = NULL;
    panel_destroy(info_p);
    info_p = NULL;
    panel_destroy(stat_p);
    stat_p = NULL;
    entity_destroy(test_e);
    map_destroy(test_map);
    base_screen_exit(self);
}

void main_screen_render()
{
    border(skills_p);
    border(stat_p);
    border(msg_p);
    border(info_p);
    panel_put(map_p, test_e->pos.x, test_e->pos.y, test_e->glyph);
}

void main_screen_handle(int key)
{
    //TODO: Main game handler
    switch (key)
    {
    case TK_KP_8:
    case TK_UP:
        try_move(test_e, 0, -1);
        break;
    case TK_KP_9:
        try_move(test_e, 1, -1);
        break;
    case TK_KP_6:
    case TK_RIGHT:
        try_move(test_e, 1, 0);
        break;
    case TK_KP_3:
        try_move(test_e, 1, 1);
        break;
    case TK_KP_2:
    case TK_DOWN:
        try_move(test_e, 0, 1);
        break;
    case TK_KP_1:
        try_move(test_e, -1, 1);
        break;
    case TK_KP_4:
    case TK_LEFT:
        try_move(test_e, -1, 0);
        break;
    case TK_KP_7:
        try_move(test_e, -1, -1);
        break;
    }
    fprintf(stdout, "Key %d pressed\n", key);
    fflush(stdout);
}

ScreenNode *screen_stack_new(Screen *s)
{
    ScreenNode *sn = malloc(sizeof(ScreenNode));
    sn->next = NULL;
    sn->screen = s;
    s->enter(s);
    return sn;
}

int screen_stack_pop(ScreenNode *first)
{
    ScreenNode *penultimate = first;
    if (first != NULL)
    {
        if (first->next == NULL)
        {
            screen_node_destroy(first);
            first = NULL;
            return 1;
        }
        else
        {
            while (penultimate->next->next != NULL)
            {
                penultimate = penultimate->next;
            }
            ScreenNode *top = penultimate->next;
            top->screen->exit(top->screen);
            screen_node_destroy(top);
            penultimate->next = NULL;
            return 0;
        }
    }
    return 1;
}

void screen_stack_push(Screen *s, ScreenNode *first)
{
    if (first == NULL)
    {
        first = screen_stack_new(s);
    }
    else
    {
        ScreenNode *top = screen_stack_peek(first);
        ScreenNode *new_node = malloc(sizeof(ScreenNode));
        new_node->screen = s;
        new_node->next = NULL;
        top->next = new_node;
        s->enter(s);
    }
}

ScreenNode *screen_stack_peek(ScreenNode *first)
{
    ScreenNode *result;
    if (first == NULL)
    {
        result = NULL;
    }
    else
    {
        result = first;
        while (result->next != NULL)
        {
            result = result->next;
        }
    }
    return result;
}

void screen_node_destroy(ScreenNode *goner)
{
    if (goner != NULL)
    {
        printf("Removing screen node with screen %s\n", goner->screen->name);
        fflush(stdout);
        free(goner->screen->name);
        free(goner->screen);
        free(goner);
    }
}

void screen_stack_destroy(ScreenNode *first)
{
    ScreenNode *cur = first;
    ScreenNode *old;
    do
    {
        old = cur;
        cur = cur->next;
        old->screen->exit(old->screen);
        screen_node_destroy(old);
    } while (cur != NULL);
}

void screen_stack_debug(ScreenNode *first)
{
    ScreenNode *cur;
    if (first == NULL)
    {
        fprintf(stderr, "Screen stack is empty\n");
        fflush(stderr);
    }
    else
    {
        cur = first;
        do
        {
            fprintf(stdout, (cur->next == NULL ? "%s\n" : "%s,"), cur->screen->name);
            fflush(stdout);
            cur = cur->next;
        } while (cur != NULL);
    }
}

void screen_stack_render(ScreenNode *first)
{
    ScreenNode *cur = first;
    while (cur != NULL)
    {
        cur->screen->render();
        cur = cur->next;
    }
}

Panel *panel_new(int x, int y, int w, int h, const char *caption)
{
    Panel *new_panel = malloc(sizeof(Panel));
    new_panel->x = x;
    new_panel->y = y;
    new_panel->w = w;
    new_panel->h = h;
    new_panel->caption = caption == NULL ? NULL : strdup(caption);
    return new_panel;
}