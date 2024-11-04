#ifndef RENDER_DESKTOP_H
#define RENDER_DESKTOP_H

#include "window.h"
#include <stddef.h>

#define MAX_WINDOWS_COUNT 128
#define DISABLED_WINDOW UINT32_MAX

typedef struct desktop_icon {
  Rectangle rec;
  Texture2D icon;
  const char *name;
  double clicked_at;
} desktop_icon;

typedef struct desktop {
  window windows[MAX_WINDOWS_COUNT];
  ui_context window_ui[MAX_WINDOWS_COUNT];
  uint32_t window_zbuf[MAX_WINDOWS_COUNT];
  size_t window_count;
  size_t id_count;

  window *fullscreen_window;
  Vector2 prefullscreen_size;
  ui_context global_ui;
  ui_context desktop_ui;

  desktop_icon icons[16];
  size_t icons_count;
} desktop;

void init_desktop(void);
void update_desktop(void);
void render_desktop(void);

desktop *get_desktop(void);

#endif
