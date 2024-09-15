#if defined(PLATFORM_DESKTOP)
#include "raylib.h"
#include "main.h"

int main(void) {
  InitWindow(1200, 800, "raylib game template");
  SetTargetFPS(60);

  init();
  while (!WindowShouldClose()) {
    main_loop();
  }
}

#endif
