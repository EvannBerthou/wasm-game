#include "utils.h"
#include "raylib.h"

Font _font = {0};

void SetDefaultFont(Font font) { _font = font; }

Font GetDefaultFont(void) {
  if (_font.glyphCount == 0) {
    return GetFontDefault();
  }
  return _font;
}
