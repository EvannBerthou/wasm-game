#include "main.h"
#include "desktop.h"
#include "raylib.h"
#include "utils.h"

void init(void) {
  InitAudioDevice();
  Font loaded_font = GetFontDefault();
  SetDefaultFont(loaded_font);
  init_desktop();
}

void main_loop(void) {
  // Updating
  update_desktop();

  // Rendering
  BeginDrawing();

  ClearBackground(RAYWHITE);
  render_desktop();

  DrawFPS(0, 0);
  EndDrawing();
}
