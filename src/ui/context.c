#include "ui/context.h"
#include "ui/button.h"
#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

void init_ui_context(ui_context *ui) {
  ui_element *head_sentinel = malloc(sizeof(ui_element));
  head_sentinel->type = UI_NONE;
  head_sentinel->rec = (Rectangle){0, 0, 0, 0};
  head_sentinel->ui_data = NULL;
  head_sentinel->state = UI_NORMAL;
  head_sentinel->prev = head_sentinel;
  ui->head = head_sentinel;
  ui->last = ui->head;
}

void update_ui_context(ui_context *ui) { (void)ui; }


// Take into account window z-indexing
void render_ui_context(ui_context *ui) {
  while (ui->last->type != UI_NONE) {
    switch (ui->last->type) {
    case UI_NONE:
      assert(false && "Unreachable");
      break;
    case UI_BUTTON:
      ui_button_render(ui->last);
      break;
    }
    ui_element *prev = ui->last->prev;
    free(ui->last);
    ui->last = prev;
  }
}

void ui_add_element(ui_context *ui, ui_element *ui_element) {
  assert(ui_element != NULL);
  ui_element->prev = ui->last;
  ui->last = ui_element;
}
