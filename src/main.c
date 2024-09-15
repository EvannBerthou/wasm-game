#include "main.h"
#include "desktop.h"
#include "raylib.h"

void init(void) { init_desktop(); }

void main_loop(void) {
  // Updating
  update_desktop();

  // Rendering
  BeginDrawing();

  ClearBackground(RAYWHITE);
  render_desktop();

  DrawFPS(100, 0);
  EndDrawing();
}
