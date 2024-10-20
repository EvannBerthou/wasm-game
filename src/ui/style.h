#ifndef UI_STYLE_H
#define UI_STYLE_H

#include "raylib.h"

#define DEFAULT_UI_PADDING 5

#define BG_COLOR GetColor(0xDCDCDCFF)
#define TEXT_COLOR GetColor(0x000000FF)
#define HOVER_COLOR GetColor(0xC8C8C8FF)
#define CLICKED_COLOR GetColor(0x636363FF)

typedef enum ui_text_styling {
  TEXT_STYLING_NONE = 0x0,
  TEXT_STYLING_CENTER = 0x1,
  TEXT_STYLING_FIT = 0x2, // Set UI element size to fit text
} ui_text_styling;

typedef struct ui_button_styling {
  Color bg_color;
  Color text_color;
  Color hover_color;
  Color clicked_color;
  ui_text_styling text_styling;
} ui_button_styling;

typedef enum UI_COLORS {
  UI_COLOR_NORMAL,
  UI_COLOR_HOVER,
  UI_COLOR_CLICKED,
  UI_COLOR_TEXT,
  UI_COLOR_COUNT,
} UI_COLORS;

#define C(r, g, b)                                                             \
  { r, g, b, 0xFF }


static const Color ui_default_colors[UI_COLOR_COUNT] = {
    [UI_COLOR_NORMAL] = C(0xDC, 0xDC, 0xDC),
    [UI_COLOR_HOVER] = C(0xC8, 0xC8, 0xC8),
    [UI_COLOR_CLICKED] = C(0x63, 0x63, 0x63),
    [UI_COLOR_TEXT] = C(0x00, 0x00, 0x00)};

#endif
