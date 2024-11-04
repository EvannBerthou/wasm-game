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


//TODO: Need to handle negatives ?
bool strtoint(const char *str, int *result) {
  int i = 0;
  bool started = false;
  while (*str != '\0') {
    if (*str >= '0' && *str <= '9') {
      if (!started)
        started = true;
      i *= 10;
      i += (*str - '0');
    } else {
      if (*str != ' ' || (*str == ' ' && !started)) {
        *result = 0;
        return false;
      }
    }
    str++;
  }
  *result = i;
  return true;
}
