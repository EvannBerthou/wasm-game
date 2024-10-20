#include "button.h"
#include "context.h"
#include "raylib.h"
#include "ui/style.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

bool ui_button_label_styled(ui_context *ui, Rectangle position,
                            const char *label, ui_text_styling styling) {
  ui_button *button = malloc(sizeof(ui_button));
  ui_element *element = &button->element;
  element->type = UI_BUTTON;
  element->rec = position;
  element->prev = NULL;
  element->state = UI_NORMAL;

  button->label = strdup(label);
  button->text_styling = styling;

  ui_add_element(ui, element);

  Rectangle *rec = &element->rec;
  Vector2 text_size = MeasureTextEx(GetDefaultFont(), button->label, 16, 1);
  if (button->text_styling & TEXT_STYLING_FIT) {
    if (text_size.x + DEFAULT_UI_PADDING * 2 > rec->width) {
      rec->width = text_size.x + DEFAULT_UI_PADDING * 2;
    }

    if (text_size.y + DEFAULT_UI_PADDING * 2 > rec->height) {
      rec->height = text_size.y + DEFAULT_UI_PADDING * 2;
    }
  }

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

bool ui_button_label(ui_context *ui, Rectangle position, const char *label) {
  return ui_button_label_styled(ui, position, label, TEXT_STYLING_CENTER);
}

bool ui_button_label_fit(ui_context *ui, Vector2 position, const char *label) {
  // Width and height will be set by ui_button_label in order to fit text
  Rectangle rec = {.x = position.x, .y = position.y, .width = 0, .height = 0};
  return ui_button_label_styled(ui, rec, label,
                                TEXT_STYLING_CENTER | TEXT_STYLING_FIT);
}

void ui_button_render(ui_button *button) {
  Color bg_color;
  switch (button->element.state) {
  case UI_NORMAL:
    bg_color = ui_default_colors[UI_COLOR_NORMAL];
    break;
  case UI_HOVER:
    bg_color = ui_default_colors[UI_COLOR_HOVER];
    break;
  case UI_CLICKED:
    bg_color = ui_default_colors[UI_COLOR_CLICKED];
    break;
  }

  Rectangle rec = button->element.rec;

  Vector2 text_position;
  if (button->text_styling & TEXT_STYLING_CENTER) {
    Vector2 text_size = MeasureTextEx(GetDefaultFont(), button->label, 16, 1);
    text_position = (Vector2){rec.x + (rec.width - text_size.x) / 2,
                              rec.y + (rec.height - text_size.y) / 2};
  } else {
    text_position = (Vector2){rec.x, rec.y};
  }

  Color text_color = ui_default_colors[UI_COLOR_TEXT];
  DrawRectangleRec(rec, bg_color);
  DrawRectangleLinesEx(rec, 2, text_color);

  DrawTextEx(GetDefaultFont(), button->label, text_position, 16, 1, text_color);
}

void free_ui_button(ui_button *button) { free((char *)button->label); }
