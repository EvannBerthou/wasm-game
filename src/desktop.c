#include "raylib.h"
#include "raymath.h"
#include "terminal.h"
#include "clock.h"
#include "window.h"

#define MAX_WINDOWS_COUNT 4

window windows[MAX_WINDOWS_COUNT] = {0};
size_t window_count = 0;

static int add_window(window w) {
  if (window_count == MAX_WINDOWS_COUNT) {
    return -1;
  }

  windows[window_count] = w;
  window_count++;
  return window_count;
}

void init_desktop() {
  window terminal1 = {.pos = {100, 100},
                      .size = {1000, 600},
                      .title = "Terminal",
                      .update = update_terminal,
                      .render = render_terminal,
                      0,
                      Vector2Zero()};
  add_window(terminal1);

  window terminal2 = {.pos = {400, 200},
                      .size = {300, 300},
                      .title = "Terminal 2",
                      .update = update_terminal,
                      .render = render_terminal,
                      0,
                      Vector2Zero()};
  add_window(terminal2);

  window clock = {
      .pos = {900, 45},
      .size = {300, 200},
      .title = "Clock",
      .update = NULL,
      .render = render_clock,
      0, Vector2Zero()
  };
  add_window(clock);
}

void update_desktop(void) {
  for (int i = 0; i < window_count; i++) {
    // TODO: Only one window should be focused
    update_window(&windows[i]);
  }
}

static void render_topbar(void) {
  DrawRectangleRec((Rectangle){0, 0, GetRenderWidth(), 35}, GRAY);
}

void render_desktop(void) {
  ClearBackground(BLACK);
  render_topbar();
  for (int i = 0; i < window_count; i++) {
    render_window(&windows[i]);
  }
}
