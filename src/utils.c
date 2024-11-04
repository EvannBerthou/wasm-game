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

// TODO: Need to handle negatives ?
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

// TODO: Possible injections (for example with filename as "../").
#if defined(PLATFORM_WEB)
const char *get_file_path(const char *base) {
  return TextFormat("/disk/%s", base);
}
#elif defined(PLATFORM_DESKTOP)
const char *get_file_path(const char *base) {
  return TextFormat("user_data/%s", base);
}
#else
#error Unsupported plateform
#endif
