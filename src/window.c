#include "window.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

// Left, Top, Right, Bottom
Vector4 paddings = {DEFAULT_PADDING, 30, DEFAULT_PADDING, DEFAULT_PADDING};

int dragging = -1;

void update_window(window *w, uint8_t id) {
  Rectangle header = {w->pos.x, w->pos.y, w->size.x, paddings.y};

  int hovering = CheckCollisionPointRec(GetMousePosition(), header);
  if (hovering && IsMouseButtonPressed(0)) {
    dragging = id;
    w->origin = Vector2Subtract(GetMousePosition(), w->pos);
  }

  if (IsMouseButtonReleased(0)) {
    dragging = -1;
    w->origin = Vector2Zero();
  }

  if (dragging == id) {
    Vector2 new_position = Vector2Subtract(GetMousePosition(), w->origin);
    w->pos = new_position;
  }

  if (w->update != NULL) {
    w->update(w);
  }
}

void render_window(window *w) {
  // BORDER
  DrawRectangleV(w->pos, w->size, LIGHTGRAY);

  // Inner
  Rectangle inner = (Rectangle){w->pos.x + paddings.x, w->pos.y + paddings.y,
                                w->size.x - paddings.x - paddings.z,
                                w->size.y - paddings.y - paddings.w};

  DrawRectangleRec(inner, BLACK);
  DrawText(w->title, INPADV(w->pos), 20, BLACK);
  if (w->render != NULL) {
    w->render(w);
  }
}

Rectangle get_window_inner(window *w) {
  Rectangle inner = {w->pos.x + paddings.x, w->pos.y + paddings.y,
                     w->size.x - paddings.x - paddings.z,
                     w->size.y - paddings.y - paddings.w};
  return inner;
}
