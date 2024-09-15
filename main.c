#include "raylib.h"
#include <stdio.h>
#include <time.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

Vector2 pos = {0};
Vector2 vel = {500, 500};

static void UpdateDrawFrame(void) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawRectangleV(pos, (Vector2){100, 75}, RED);
  if (pos.x < 0 || pos.x > GetRenderWidth() - 100)
    vel.x *= -1;
  if (pos.y < 0 || pos.y > GetRenderHeight() - 75)
    vel.y *= -1;
  pos.x += vel.x * (1.f / 60.f);
  pos.y += vel.y * (1.f / 60.f);

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  DrawText(TextFormat("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900,
                      tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
                      tm.tm_sec),
           0, 0, 24, BLACK);
  EndDrawing();
}

int main(void) {
  InitWindow(1200, 800, "raylib game template");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }
#endif
}
