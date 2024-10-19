#include "window.h"
#include "raylib.h"
#include "raymath.h"
#include "ui/button.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Left, Top, Right, Bottom
Vector4 paddings = {DEFAULT_PADDING, 30, DEFAULT_PADDING, DEFAULT_PADDING};

uint32_t dragging = UINT32_MAX;

static Rectangle get_border(window *w) {
  return (Rectangle){w->pos.x - paddings.x, w->pos.y - paddings.y,
                     w->size.x + paddings.x + paddings.z,
                     w->size.y + paddings.y + paddings.w};
}

static Rectangle get_header(window *w) {
  return (Rectangle){w->pos.x - paddings.x + DEFAULT_PADDING,
                     w->pos.y - paddings.y + DEFAULT_PADDING,
                     w->size.x + paddings.x + paddings.z - DEFAULT_PADDING,
                     paddings.y + paddings.w - DEFAULT_PADDING * 2 - 2};
}

void init_window(window *w, Vector2 pos, Vector2 size, const char *title) {
  w->pos = pos;
  w->size = size;
  strncpy(w->title, title, 31);
  w->title[31] = '\0';
  w->window_data = NULL;
}

uint32_t update_window(window *w, ui_context *ui) {
  Rectangle header = get_header(w);

  int hovering = CheckCollisionPointRec(GetMousePosition(), header);
  if (hovering && IsMouseButtonPressed(0) && dragging == UINT32_MAX) {
    dragging = w->id;
    w->origin = Vector2Subtract(GetMousePosition(), w->pos);
  }

  if (IsMouseButtonReleased(0)) {
    dragging = UINT32_MAX;
    w->origin = Vector2Zero();
  }

  if (dragging == w->id) {
    Vector2 new_position = Vector2Subtract(GetMousePosition(), w->origin);
    w->pos = new_position;
  }

  if (w->update != NULL) {
    w->update(w);
  }

  // We need to update the header position in case the window moves. If we
  // don't, we will draw the UI at a previous window position
  header = get_header(w);
  Color c = GetColor(0xDCDCDCFF);
  const int BUTTON_SIZE = 20;
  Rectangle close_rec = {header.x + header.width - DEFAULT_PADDING -
                             BUTTON_SIZE,
                         header.y, BUTTON_SIZE, BUTTON_SIZE};
  if (ui_button_label(ui, close_rec, "X", c, BLACK, LIGHTGRAY)) {
    printf("Close\n");
  }

  Rectangle fullscreen_rec = {header.x + header.width -
                                  (DEFAULT_PADDING + BUTTON_SIZE) * 2,
                              header.y, BUTTON_SIZE, BUTTON_SIZE};
  if (ui_button_label(ui, fullscreen_rec, "O", c, BLACK, LIGHTGRAY)) {
    printf("Fullscreen\n");
  }

  return dragging;
}

void render_window(window *w) {
  // Border
  Color c = GetColor(0xDCDCDCFF);

  Rectangle border = get_border(w);
  DrawRectangleRec(border, c);

  // Header
  Rectangle header = get_header(w);
  DrawRectangleRec(header, c);
  Color start = GetColor(0x909090FF);
  DrawRectangleGradientEx(header, start, start, c, c);
  DrawTextEx(GetDefaultFont(), w->title,
             (Vector2){header.x + DEFAULT_PADDING, header.y}, 20, 3, BLACK);

  // Window
  if (w->render != NULL) {
    BeginTextureMode(w->target);
    ClearBackground(BLACK);
    w->render(w);
    EndTextureMode();
  }
}

void disable_dragging() { dragging = UINT32_MAX; }
