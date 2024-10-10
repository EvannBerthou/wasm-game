#include "window.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <string.h>

// Left, Top, Right, Bottom
Vector4 paddings = {DEFAULT_PADDING, 30, DEFAULT_PADDING, DEFAULT_PADDING};

int dragging = -1;

static Rectangle get_border(window *w) {
  return (Rectangle){w->pos.x - paddings.x, w->pos.y - paddings.y,
                     w->size.x + paddings.x + paddings.z,
                     w->size.y + paddings.y + paddings.w};
}

void init_window(window *w, Vector2 pos, Vector2 size, const char *title) {
  w->pos = pos;
  w->size = size;
  strncpy(w->title, title, 31);
  w->title[31] = '\0';
}

int update_window(window *w) {
  Rectangle header = get_border(w);

  int hovering = CheckCollisionPointRec(GetMousePosition(), header);
  if (hovering && IsMouseButtonPressed(0) && dragging == -1) {
    dragging = w->id;
    w->origin = Vector2Subtract(GetMousePosition(), w->pos);
  }

  if (IsMouseButtonReleased(0)) {
    dragging = -1;
    w->origin = Vector2Zero();
  }

  if (dragging == w->id) {
    Vector2 new_position = Vector2Subtract(GetMousePosition(), w->origin);
    w->pos = new_position;
  }

  if (w->update != NULL) {
    w->update(w);
  }
  return dragging;
}

void render_window(window *w) {
  // BORDER
  Rectangle border = get_border(w);
  DrawRectangleRec(border, LIGHTGRAY);

  DrawText(w->title, INPADV(border), 20, BLACK);
  if (w->render != NULL) {
    BeginTextureMode(w->target);
    ClearBackground(BLACK);
    w->render(w);
    EndTextureMode();
  }
}
