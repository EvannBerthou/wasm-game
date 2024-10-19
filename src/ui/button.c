#include "button.h"
#include "context.h"
#include "utils.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

bool ui_button_label(ui_context *ui, Rectangle position, const char *label,
                     Color bg_color, Color text_color, Color hover_color) {
  ui_element *element = malloc(sizeof(ui_element));
  element->type = UI_BUTTON;
  element->rec = position;
  element->prev = NULL;
  element->state = UI_NORMAL;

  ui_button_data *data = malloc(sizeof(ui_button_data));
  data->label = strdup(label);
  data->bg_color = bg_color;
  data->text_color = text_color;
  data->hover_color = hover_color;
  element->ui_data = data;

  ui_add_element(ui, element);

  if (CheckCollisionPointRec(GetMousePosition(), element->rec)) {
    element->state = UI_HOVER;

    if (IsMouseButtonDown(0)) {
      element->state = UI_CLICKED;
    }

    // Only trigger clicked event once
    if (IsMouseButtonPressed(0)) {
      return true;
    }
  }

  return false;
}

void ui_button_render(ui_element *element) {
  ui_button_data *data = (ui_button_data *)element->ui_data;

  Color c;
  switch (element->state) {
  case UI_NORMAL:
    c = data->bg_color;
    break;
  case UI_HOVER:
    c = data->hover_color;
    break;
  case UI_CLICKED:
    c = data->text_color;
    break;
  }

  DrawRectangleRec(element->rec, c);
  DrawRectangleLinesEx(element->rec, 2, data->text_color);
  Vector2 text_position = (Vector2){element->rec.x + 5, element->rec.y + 3};
  DrawTextEx(GetDefaultFont(), data->label, text_position, 16, 1,
             data->text_color);
}
