#include "apps/simon/main.h"
#include "raylib.h"
#include "utils.h"
#include "window.h"
#include <stdlib.h>

#define ball_count 1024 * 5

Texture2D anniv;
Texture2D anniv2;
Texture2D duo;
Vector2 *balls;
Color *colors;
Music music;

void init_balls() {
  for (int i = 0; i < ball_count; i++) {
    int r = GetRandomValue(0, 255);
    int g = GetRandomValue(0, 255);
    int b = GetRandomValue(0, 255);
    colors[i] = (Color){r, g, b, 0xFF};
    balls[i] = (Vector2){GetRandomValue(0, 1200), GetRandomValue(-500, 10)};
  }
}

window new_simon(int posx, int posy, int sizex, int sizey, const char *title) {
  balls = malloc(sizeof(Vector2) * ball_count);
  colors = malloc(sizeof(Color) * ball_count);
  anniv = LoadTextureFromImage(LoadImage("img/anniv.png"));
  anniv2 = LoadTextureFromImage(LoadImage("img/anniv2.png"));
  duo = LoadTextureFromImage(LoadImage("img/duo.png"));
  music = LoadMusicStream("img/music.mp3");
  init_balls();

  window w = {.render = render_simon};
  init_window(&w, (Vector2){posx, posy}, (Vector2){sizex, sizey}, title);
  return w;
}

static int i = 0;
int playing = 0;

void render_simon(window *w) {
  if (playing == 0) {
    PlayMusicStream(music);
    playing = 1;
  }

  UpdateMusicStream(music);
  (void)w;
  i++;
  if (i % 50 < 25) {
    ClearBackground(RED);
  } else {
    ClearBackground(BLUE);
  }
  DrawTexture(anniv, 10, 10, WHITE);
  DrawTexture(anniv2, 920, 10, WHITE);
  DrawTexture(duo, 300, 100, WHITE);

  DrawTextEx(GetDefaultFont(), "POUR SIM'S", (Vector2){375, 25}, 72, 3, WHITE);

  Vector2 s =
      MeasureTextEx(GetDefaultFont(), "JOYEUX ANNIVERSAIRE !!!!", 40, 3);
  DrawTextPro(GetDefaultFont(), "JOYEUX ANNIVERSAIRE !!!!",
              (Vector2){325 + s.x / 2, 400 + s.y / 2},
              (Vector2){s.x / 2, s.y / 2}, i * 3, 40, 3,
              i % 50 < 25 ? BLUE : RED);

  for (int i = 0; i < ball_count; i++) {
    balls[i].y += GetRandomValue(0, 10);
    balls[i].x += GetRandomValue(-1, 1);
    DrawCircleV(balls[i], 15, colors[i]);
  }
  if (balls[0].y > 2000) {
    init_balls();
  }
  if (GetMusicTimeLength(music) == GetMusicTimePlayed(music)) {
    playing = 0;
  }
}
