#include "screen.h"
#include "BearLibTerminal.h"
#include "entity.h"
#include "mapgen.h"
#include "new.h"
#include "stdio.h"
#include <string.h>

static Panel* msg_p;
static Panel* map_p;
static Panel* stat_p;
static Panel* info_p;
static Panel* skills_p;
static Entity* test_e;
static GameMap* test_map;

static void handle_alloc_error(const char* name) {
    fprintf(stderr, "Failed to allocate new %s screen", name);
    fflush(stderr);
    exit(1);
}

static void write_center(const char* text, int y) {
    int w = terminal_measure(text).width;
    if (w > SCREEN_W) {
        fprintf(stderr, "Line %s is too long", text);
        fflush(stderr);
        exit(1);
    }
    terminal_print((SCREEN_W - w) / 2, y, text);
}

static dimensions_t panel_print(Panel* panel, int x, int y, const char* text) {
    return terminal_print(x + panel->x, y + panel->y, text);
}

static void panel_put(Panel* panel, int x, int y, int code, const char* color) {
    if (color != NULL) {
        terminal_color(color_from_name(color));
    }
    terminal_put(x + panel->x, y + panel->y, code);
    terminal_color(color_from_name("white"));
}

static void border(Panel* panel) {
    static int UL = 0x2554;
    static int UR = 0x2557;
    static int LL = 0x255A;
    static int LR = 0x255D;
    static int HORZ = 0x2550;
    static int VERT = 0x2551;
    int x2 = panel->w - 1;
    int y2 = panel->h - 1;

    for (int xs = 1; xs < x2; xs++) {
        panel_put(panel, xs, 0, HORZ, NULL);
        panel_put(panel, xs, y2, HORZ, NULL);
    }

    for (int ys = 1; ys < y2; ys++) {
        panel_put(panel, 0, ys, VERT, NULL);
        panel_put(panel, x2, ys, VERT, NULL);
    }

    panel_put(panel, 0, 0, UL, NULL);
    panel_put(panel, x2, 0, UR, NULL);
    panel_put(panel, 0, y2, LL, NULL);
    panel_put(panel, x2, y2, LR, NULL);

    if (panel->caption != NULL) {
        panel_print(panel, 1, 0, panel->caption);
    }
}

static void panel_destroy(Panel* p) {
    printf("Removing panel");
    printf((p->caption == NULL ? "\n" : " with caption %s\n"), p->caption);
    fflush(stdout);
    free(p->caption);
    p->caption = NULL;
    free(p);
}

Screen* title_screen_new(void) {
    Screen* s = NEW(Screen, 1);
    if (!s) {
        handle_alloc_error("title");
    }
    s->enter = base_screen_enter;
    s->exit = base_screen_exit;
    s->handle = title_screen_handle;
    s->name = strdup("title");
    s->render = title_screen_render;
    return s;
}

Screen* main_screen_new(void) {
    Screen* s = NEW(Screen, 1);
    if (!s) {
        handle_alloc_error("title");
    }
    s->enter = main_screen_enter;
    s->exit = main_screen_exit;
    s->handle = main_screen_handle;
    s->name = strdup("main");
    s->render = main_screen_render;

    return s;
}

void base_screen_enter(Screen* self) {
    fprintf(stdout, "Entered %s screen\n", self->name);
    fflush(stdout);
}

void base_screen_exit(Screen* self) {
    fprintf(stdout, "Exited %s screen\n", self->name);
    fflush(stdout);
}

void title_screen_render() {
    write_center("Wolf's Den 2: C Edition", 15);
    write_center("by Rakaneth", 16);
    write_center("Press any key to start", 18);
}

void title_screen_handle(int key) {
    if (key != TK_CLOSE) {
        screen_stack_pop(screen_stack);
        screen_stack = screen_stack_new(main_screen_new());
    }
}

void main_screen_enter(Screen* self) {
    // TODO: chargen
    msg_p = panel_new(MSG_X, MSG_Y, MSG_W, MSG_H, "Messages");
    map_p = panel_new(MAP_X, MAP_Y, MAP_W, MAP_H, NULL);
    stat_p = panel_new(STAT_X, STAT_Y, STAT_W, STAT_H, "Stats");
    info_p = panel_new(INFO_X, INFO_Y, INFO_W, INFO_H, "Info");
    skills_p = panel_new(SKL_X, SKL_Y, SKL_W, SKL_H, "Skills");
    test_e = entity_new('@', "Player", "The player!", ENTITY_CREATURE);
    test_map = generate_map("mine");
    // map_debug(test_map);
}

void main_screen_exit(Screen* self) {
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

static Point screen_point(Point p) {
    Point new_p;
    new_p.x = p.x % MAP_W;
    new_p.y = p.y % MAP_H;
    return new_p;
}

// Screen scrolls on reaching the bottom of the screen
static Point cam(Point p) {
    Point new_p;

    /* abusing integer division to get clean rounding to the nearest multiple of
     * the map dimensions */

    new_p.x = MAP_W * (p.x / MAP_W);
    new_p.y = MAP_H * (p.y / MAP_H);
    return new_p;
}

static void render_map() {
    Point c = cam(test_e->pos);
    Point screen_p, ps_p;
    Tile t;

    for (int x = c.x; x < c.x + MAP_W; x++) {
        for (int y = c.y; y < c.y + MAP_H; y++) {
            t = map_get_tile(test_map, x, y);
            if (strcmp(t.name, "null") != 0) {
                screen_p = screen_point((Point){x, y});
                panel_put(map_p, screen_p.x, screen_p.y, t.glyph,
                          test_map->wall_color);
            }
        }
    }

    ps_p = screen_point(test_e->pos);
    panel_put(map_p, ps_p.x, ps_p.y, test_e->glyph, test_e->color);
}

static void render_skills() {
    border(skills_p);
}

static void render_stats() {
    char buf[50];
    border(stat_p);
    panel_print(stat_p, 1, 1, test_e->name);
    sprintf(buf, "(%d, %d) - %s", test_e->pos.x, test_e->pos.y, test_map->name);
    panel_print(stat_p, 1, 2, buf);
}

static void render_msgs() {
    border(msg_p);
}

static void render_info() {
    border(info_p);
}

void main_screen_render() {
    render_map();
    render_skills();
    render_stats();
    render_msgs();
    render_info();
}

void main_screen_handle(int key) {
    // TODO: Main game handler
    switch (key) {
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

ScreenNode* screen_stack_new(Screen* s) {
    ScreenNode* sn = NEW(ScreenNode, 1);
    sn->next = NULL;
    sn->screen = s;
    s->enter(s);
    return sn;
}

int screen_stack_pop(ScreenNode* first) {
    ScreenNode* penultimate = first;
    if (first != NULL) {
        if (first->next == NULL) {
            screen_node_destroy(first);
            first = NULL;
            return 1;
        } else {
            while (penultimate->next->next != NULL) {
                penultimate = penultimate->next;
            }
            ScreenNode* top = penultimate->next;
            top->screen->exit(top->screen);
            screen_node_destroy(top);
            penultimate->next = NULL;
            return 0;
        }
    }
    return 1;
}

void screen_stack_push(Screen* s, ScreenNode* first) {
    if (first == NULL) {
        first = screen_stack_new(s);
    } else {
        ScreenNode* top = screen_stack_peek(first);
        ScreenNode* new_node = NEW(ScreenNode, 1);
        new_node->screen = s;
        new_node->next = NULL;
        top->next = new_node;
        s->enter(s);
    }
}

ScreenNode* screen_stack_peek(ScreenNode* first) {
    ScreenNode* result;
    if (first == NULL) {
        result = NULL;
    } else {
        result = first;
        while (result->next != NULL) {
            result = result->next;
        }
    }
    return result;
}

void screen_node_destroy(ScreenNode* goner) {
    if (goner != NULL) {
        printf("Removing screen node with screen %s\n", goner->screen->name);
        fflush(stdout);
        free(goner->screen->name);
        free(goner->screen);
        free(goner);
    }
}

void screen_stack_destroy(ScreenNode* first) {
    ScreenNode* cur = first;
    ScreenNode* old;
    do {
        old = cur;
        cur = cur->next;
        old->screen->exit(old->screen);
        screen_node_destroy(old);
    } while (cur != NULL);
}

void screen_stack_debug(ScreenNode* first) {
    ScreenNode* cur;
    if (first == NULL) {
        fprintf(stderr, "Screen stack is empty\n");
        fflush(stderr);
    } else {
        cur = first;
        do {
            fprintf(stdout, (cur->next == NULL ? "%s\n" : "%s,"),
                    cur->screen->name);
            fflush(stdout);
            cur = cur->next;
        } while (cur != NULL);
    }
}

void screen_stack_render(ScreenNode* first) {
    ScreenNode* cur = first;
    while (cur != NULL) {
        cur->screen->render();
        cur = cur->next;
    }
}

Panel* panel_new(int x, int y, int w, int h, const char* caption) {
    Panel* new_panel = NEW(Panel, 1);
    new_panel->x = x;
    new_panel->y = y;
    new_panel->w = w;
    new_panel->h = h;
    new_panel->caption = caption == NULL ? NULL : strdup(caption);
    return new_panel;
}