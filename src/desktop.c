#include "apps/Dungeon/main.h"
#include "apps/clock/main.h"
#include "apps/terminal/main.h"
#include "raylib.h"
#include "raymath.h"
#include "window.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WINDOWS_COUNT 128
#define DISABLED_WINDOW UINT32_MAX

window windows[MAX_WINDOWS_COUNT] = {0};
uint32_t window_zbuf[MAX_WINDOWS_COUNT] = {-1};
size_t window_count = 0;
size_t WINDOW_IDS = 0;

window *fullscreen_window = NULL;
Vector2 prefullscreen_size;

static window *get_window_with_id(uint32_t id) {
  assert(id < DISABLED_WINDOW);
  for (uint32_t i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (windows[i].id == id) {
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
  WINDOW_IDS = (WINDOW_IDS + 1) % UINT32_MAX;
  w.target = LoadRenderTexture(w.size.x, w.size.y);

  if (window_count > 0) {
    get_focused_window()->focused = false;
  }
  w.focused = true;

  if (w.init) {
    w.init(&w);
  }

  for (int i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (windows[i].id == DISABLED_WINDOW) {
      windows[i] = w;
      window_zbuf[window_count] = w.id;
      break;
    }
  }

  window_count++;
  return window_count;
}

static void set_top_window(uint32_t id) {
  int c1 = 0;
  for (size_t i = 0; i < window_count; i++) {
    if (window_zbuf[i] == id)
      continue;
    window_zbuf[c1] = window_zbuf[i];
    c1++;
  }
  window_zbuf[window_count - 1] = id;
}

static void kill_top_window() {
  if (window_count == 0) {
    return;
  }

  window *w = get_focused_window();
  printf("Killing %s with ID=%u\n", w->title, w->id);
  if (w == fullscreen_window) {
    fullscreen_window = NULL;
    prefullscreen_size = Vector2Zero();
  }

  if (w->window_data != NULL) {
    free(w->window_data);
  }
  w->window_data = NULL;

  disable_dragging();
  UnloadRenderTexture(w->target);
  w->id = DISABLED_WINDOW;

  window_count--;
  window_zbuf[window_count] = 0;
  if (window_count > 0) {
    set_top_window(window_zbuf[window_count - 1]);
  }
}

void init_desktop() {
  for (int i = 0; i < MAX_WINDOWS_COUNT; i++) {
    windows[i].id = DISABLED_WINDOW;
  }

  add_window(new_terminal(100, 100, 1000, 600, "Terminal"));
  add_window(new_terminal(400, 200, 300, 300, "Terminal 2"));
  add_window(new_dungeon(100, 100, 960, 540, "Dungeon"));
  add_window(new_dungeon(200, 100, 960, 540, "Dungeon"));
  add_window(new_clock(900, 45, 300, 200, "Clock"));
}

void update_desktop(void) {
  if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_T)) {
    const char *name = TextFormat("Terminal %d", WINDOW_IDS);
    add_window(new_terminal(100, 100, 1000, 600, name));
  }

  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)) {
    kill_top_window();
  }

  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) {
    if (fullscreen_window) {
      window *w = fullscreen_window;
      w->size = prefullscreen_size;
      UnloadRenderTexture(w->target);
      w->target = LoadRenderTexture(w->size.x, w->size.y);

      fullscreen_window = NULL;
      prefullscreen_size = Vector2Zero();
    } else {
      fullscreen_window = get_focused_window();
      prefullscreen_size = fullscreen_window->size;

      fullscreen_window->size.x = 1200;
      fullscreen_window->size.y = 800;
      UnloadRenderTexture(fullscreen_window->target);
      fullscreen_window->target = LoadRenderTexture(1200, 800);
    }
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

  if (window_count > 0) {
    for (int i = window_count; i != 0; i--) {
      window *w = get_window_with_id(window_zbuf[i - 1]);
      uint32_t res = update_window(w);
      if (res != UINT32_MAX) {
        get_focused_window()->focused = false;
        set_top_window(res);
        get_focused_window()->focused = true;
      }
    }
  }
}

static void render_topbar(void) {
  DrawRectangleRec((Rectangle){0, 0, GetRenderWidth(), 35}, GRAY);
}

void render_desktop(void) {
  ClearBackground(RAYWHITE);
  render_topbar();

  for (size_t i = 0; i < window_count; i++) {
    window *w = get_window_with_id(window_zbuf[i]);

    if (fullscreen_window && w != fullscreen_window)
      continue;

    render_window(w);

    Vector2 origin;
    if (fullscreen_window) {
      origin = Vector2Zero();
    } else {
      origin = w->pos;
    }
    DrawTextureRec(w->target.texture, (Rectangle){0, 0, w->size.x, -w->size.y},
                   origin, WHITE);
  }
}
