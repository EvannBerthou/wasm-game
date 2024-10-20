#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui/context.h"

typedef struct ui_button_data {
  const char *label;
  Color bg_color;
  Color text_color;
  Color hover_color;
  Color clicked_color;
} ui_button_data;

bool ui_button_label_styled(ui_context *ui, Rectangle position,
                            const char *label, Color bg_color, Color text_color,
                            Color hover_color, Color clicked_color);
bool ui_button_label(ui_context *ui, Rectangle position, const char *label);
void ui_button_render(ui_element *element);

#endif
