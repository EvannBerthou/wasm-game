#ifndef WINDOW_H
#define WINDOW_H

#include "raylib.h"
#include "ui/context.h"
#include <stddef.h>
#include <stdint.h>

#define DEFAULT_PADDING 5.f
#define PAD(x) (x) + DEFAULT_PADDING
// Inline Padding Vector
#define INPADV(v) ((v.x) + DEFAULT_PADDING), ((v.y) + DEFAULT_PADDING)

#define MAX_WINDOW_MESSAGE_COUNT 4
#define MAX_WINDOW_MESSAGE_LENGTH 128

typedef enum window_update_action {
    ACTION_NONE,
    ACTION_KILL,
    ACTION_FULLSCREEN,
    ACTION_SELECT
} window_update_action;

typedef struct window {
  uint32_t id;
  Vector2 pos;
  Vector2 size;
  char title[32];

  void (*init)(struct window *);
  void (*update)(struct window *);
  void (*render)(struct window *);
  void (*free)(struct window *);

  Vector2 origin;

  void *window_data;
  bool focused;
  RenderTexture target;

  char messages[MAX_WINDOW_MESSAGE_COUNT][MAX_WINDOW_MESSAGE_LENGTH];
  size_t message_count;
} window;

void init_window(window *w, Vector2 pos, Vector2 size, const char *title);
window_update_action update_window(window *w, ui_context *ui);
void render_window(window *w);
void disable_dragging(void);

void send_window_message(window *w, const char *msg);
const char *read_next_message(window *w);

#endif
