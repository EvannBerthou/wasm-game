#include "raylib.h"
#include "terminal.h"

void init_desktop() {
    Vector2 terminal_position = {100, 100};
    Vector2 terminal_size = {1000, 600};
    init_terminal(terminal_position, terminal_size);
}

void update_desktop(void) {
    update_terminal();
}

static void render_topbar(void) {
    DrawRectangleRec((Rectangle){0, 0, GetRenderWidth(), 35}, GRAY);
}

void render_desktop(void) {
    ClearBackground(BLACK);
    render_topbar();
    render_terminal();
}
