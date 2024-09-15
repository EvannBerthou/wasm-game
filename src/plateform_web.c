#if defined(PLATFORM_WEB)
#include "raylib.h"
#include "main.h"
#include <emscripten/emscripten.h>

int main() {
  InitWindow(1200, 800, "raylib game template");
  init();
  emscripten_set_main_loop(main_loop, 60, 1);
}
#endif
