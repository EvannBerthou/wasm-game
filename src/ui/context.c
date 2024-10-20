#include "ui/context.h"
#include "raylib.h"
#include "ui/button.h"
#include <assert.h>
#include <raymath.h>
#include <stdlib.h>

void init_ui_context(ui_context *ui) {
  ui_element *head_sentinel = malloc(sizeof(ui_element));
  head_sentinel->type = UI_NONE;
  head_sentinel->rec = (Rectangle){0, 0, 0, 0};
  head_sentinel->state = UI_NORMAL;
  head_sentinel->prev = head_sentinel;
  ui->head = head_sentinel;
  ui->last = ui->head;

  ui->prev_mouse = Vector2Zero();
}

void update_ui_context(ui_context *ui) {
  ui->prev_mouse = GetMousePosition();
  if (IsMouseButtonPressed(0)) {
      ui->down_at = GetMousePosition();
  }
}

void render_ui_context(ui_context *ui) {
  while (ui->last != ui->head) {
    switch (ui->last->type) {
    case UI_NONE:
      assert(false && "Unreachable");
      break;
    case UI_BUTTON: {
      ui_button *button = (ui_button *)ui->last;
      ui_button_render(button);
      free_ui_button(button);
      break;
    }
    }
    ui_element *prev = ui->last->prev;
    free(ui->last);
    ui->last = prev;
  }
}

void free_ui_context(ui_context *ui) {
  while (ui->last->type != UI_NONE) {
    ui_element *prev = ui->last->prev;
    free(ui->last);
    ui->last = prev;
  }
  free(ui->head);
  free(ui);
}

void ui_add_element(ui_context *ui, ui_element *ui_element) {
  assert(ui_element != NULL);
  ui_element->prev = ui->last;
  ui->last = ui_element;
}
