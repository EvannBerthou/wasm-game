#include "raylib.h"
#include "window.h"
#include <time.h>

void render_clock(window *w) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  const char *text =
      TextFormat("%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900,
                 tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);


  Rectangle inner = get_window_inner(w);
  DrawRectangleRec(inner, GREEN);

  Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 28, 2);
  int x = inner.x + (inner.width - text_size.x) / 2;
  int y = inner.y + (inner.height - text_size.y) / 2 + 14;
  DrawText(text, x, y, 28, WHITE);
}
