#include "Dungeon/src/main.h"
#include "clock.h"
#include "raylib.h"
#include "raymath.h"
#include "terminal.h"
#include "window.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WINDOWS_COUNT 16

window windows[MAX_WINDOWS_COUNT] = {0};
bool window_active[MAX_WINDOWS_COUNT] = {0};
int window_zbuf[MAX_WINDOWS_COUNT] = {-1};
size_t window_count = 0;
size_t WINDOW_IDS = 0;

static window *get_window_with_id(int id) {
  for (int i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (window_active[i] && windows[i].id == id) {
      return &windows[i];
    }
  }
  assert(false && "Should not be reached");
}

static window *get_focused_window() {
  return get_window_with_id(window_zbuf[window_count - 1]);
}

static int add_window(window w) {
  if (window_count == MAX_WINDOWS_COUNT) {
    return -1;
  }

  w.id = WINDOW_IDS;
  WINDOW_IDS++;
  w.window_data = NULL;
  w.target = LoadRenderTexture(w.size.x, w.size.y);

  if (window_count > 0) {
    get_focused_window()->focused = false;
  }
  w.focused = true;

  if (w.init) {
    w.init(&w);
  }

  for (int i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (window_active[i] == 0) {
      window_active[i] = 1;
      windows[i] = w;
      window_zbuf[window_count] = w.id;
      break;
    }
  }
  window_count++;
  return window_count;
}

static void set_top_window(int id) {
  int i = 0;
  while (window_zbuf[i] != id) {
    i++;
  }
  for (; i < window_count - 1; i++) {
    window_zbuf[i] = window_zbuf[i + 1];
  }
  window_zbuf[window_count - 1] = id;
}

static void kill_top_window() {
  if (window_count == 0) {
    return;
  }

  window *w = get_focused_window();
  if (w->window_data) {
    free(w->window_data);
  }

  for (int i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (windows[i].id == w->id) {
      printf("Killing %s with ID=%d\n", w->title, i);
      window_active[i] = 0;
      memset(&windows[i], 0, sizeof(window));
      window_count--;
      if (window_count > 0) {
        window_zbuf[window_count] = -1;
        windows[window_zbuf[window_count - 1]].focused = true;
      }
      break;
    }
  }
}

void init_desktop() {
  window terminal1 = {.pos = {100, 100},
                      .size = {1000, 600},
                      .title = "Terminal",
                      .init = init_terminal,
                      .update = update_terminal,
                      .render = render_terminal,
                      Vector2Zero()};
  add_window(terminal1);

  window terminal2 = {.pos = {400, 200},
                      .size = {300, 300},
                      .title = "Terminal 2",
                      .init = init_terminal,
                      .update = update_terminal,
                      .render = render_terminal,
                      Vector2Zero()};
  add_window(terminal2);

  window clock = {.pos = {900, 45},
                  .size = {300, 200},
                  .title = "Clock",
                  .update = NULL,
                  .render = render_clock,
                  Vector2Zero()};
  add_window(clock);

  window dungeon = {.pos = {100, 100},
                    .size = {960, 540},
                    .title = "Dungeon",
                    .init = init_dungeon,
                    .update = update_dungeon,
                    .render = render_dungeon};
  add_window(dungeon);
}

void update_desktop(void) {
  if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_T)) {
    const char *name = TextFormat("Terminal %d", window_count);
    window terminal = {.pos = {100, 100},
                       .size = {500, 300},
                       .init = init_terminal,
                       .update = update_terminal,
                       .render = render_terminal,
                       Vector2Zero()};
    // TODO: Cleaner way
    memcpy(terminal.title, name, strlen(name));
    add_window(terminal);
  }

  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)) {
    kill_top_window();
  }

  if (IsKeyPressed(KEY_R)) {
    int sign = 1;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
      sign = -1;
    }
    window *w = get_focused_window();
    w->size.x += 100 * sign;
    w->size.y += 100 * sign;
    UnloadRenderTexture(w->target);
    w->target = LoadRenderTexture(w->size.x, w->size.y);
  }

  for (int i = MAX_WINDOWS_COUNT - 1; i >= 0; i--) {
    if (window_active[i] == 0)
      continue;
    int res = update_window(&windows[i]);
    if (res != -1 && res == windows[i].id) {
      get_focused_window()->focused = false;
      set_top_window(res);
      get_focused_window()->focused = true;
    }
  }
}

static void render_topbar(void) {
  DrawRectangleRec((Rectangle){0, 0, GetRenderWidth(), 35}, GRAY);
}

void render_desktop(void) {
  ClearBackground(RAYWHITE);
  render_topbar();
  for (int i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (window_active[i] == 0)
      continue;
    window *w = &windows[window_zbuf[i]];
    render_window(w);

    DrawTextureRec(w->target.texture, (Rectangle){0, 0, w->size.x, -w->size.y},
                   (Vector2){w->pos.x, w->pos.y}, WHITE);
  }
}
