
#include "BearLibTerminal.h"
#include <stdio.h>
#include "screen.h"

int main()
{
    screen_stack = screen_stack_new(title_screen_new());
    int key;
    terminal_open();

    do
    {
        terminal_clear();
        screen_stack_render(screen_stack);
        terminal_refresh();
        key = terminal_read();
        screen_stack_peek(screen_stack)->screen->handle(key);
    } while (key != TK_CLOSE);

    screen_stack_destroy(screen_stack);
    terminal_close();
    getchar();
    return 0;
}