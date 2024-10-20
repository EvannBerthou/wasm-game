#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui/context.h"
#include "ui/style.h"

typedef struct ui_button {
  ui_element element;
  const char *label;
  ui_text_styling text_styling;
} ui_button;

bool ui_button_label_styled(ui_context *ui, Rectangle position,
                            const char *label, ui_text_styling styling);
bool ui_button_label(ui_context *ui, Rectangle position, const char *label);
bool ui_button_label_fit(ui_context *ui, Vector2 position, const char *label);
void ui_button_render(ui_button *button);
void free_ui_button(ui_button *button);

#endif
