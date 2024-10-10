#ifndef WINDOW_H
#define WINDOW_H

#include "raylib.h"
#include <stdint.h>

#define DEFAULT_PADDING 5.f
#define PAD(x) (x) + DEFAULT_PADDING
// Inline Padding Vector
#define INPADV(v) ((v.x) + DEFAULT_PADDING), ((v.y) + DEFAULT_PADDING)

typedef struct window {
  uint8_t id;
  Vector2 pos;
  Vector2 size;
  char title[32];

  void (*init)(struct window *);
  void (*update)(struct window *);
  void (*render)(struct window *);

  Vector2 origin;

  void *window_data;
  bool focused;
  RenderTexture target;
} window;

void init_window(window *w, Vector2 pos, Vector2 size, const char *title);
int update_window(window *w);
void render_window(window *w);

Rectangle get_window_inner(window *w);

#endif
