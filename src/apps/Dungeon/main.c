#include "main.h"
#include "player_control.h"
#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "stddef.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int screenWidth = 960;
const int screenHeight = 540;
const float turnDuration = 0.3;
const float forwardDuration = 0.3;

window new_dungeon(int posx, int posy, int sizex, int sizey,
                   const char *title) {
  window w = {
      .init = init_dungeon, .update = update_dungeon, .render = render_dungeon};
  init_window(&w, (Vector2){posx, posy}, (Vector2){sizex, sizey}, title);
  w.window_data = malloc(sizeof(dungeon_data));
  return w;
}

void init_dungeon(window *w) {
  dungeon_data *data = (dungeon_data *)w->window_data;
  data->gameState = IN_GAME;
  data->animState = NONE;
  data->timeTurnAnim = turnDuration;
  data->oldAngle = 0;
  data->rotateSum = 0;
  data->gauche = false;
  data->timeForwardAnim = forwardDuration;
  int map[MAP_SIZE_X][MAP_SIZE_Y] = {{1, 1, 1, 2, 1}, // 1=floor
                                     {1, 2, 1, 2, 1}, // 2=wall
                                     {1, 1, 1, 2, 1},
                                     {2, 1, 1, 1, 1},
                                     {2, 2, 1, 2, 2}};
  memcpy(data->map, map, sizeof map);

  data->npcNumber = 2;
  data->isFacingNpc = false;

  data->camera.position = (Vector3){0.0f, 2.0f, 0.0f}; // Camera position
  data->camera.target = (Vector3){0.0f, 2.0f, 1.0f}; // Camera looking at point
  data->camera.up =
      (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
  data->camera.fovy = 90.0f;       // Camera field-of-view Y
  data->camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

  data->turnData.camera = &data->camera;
  data->turnData.targetTime = turnDuration;
  data->turnData.currentTime = data->timeTurnAnim;
  data->turnData.oldValue = data->oldAngle;
  data->turnData.sum = data->rotateSum;

  data->forwardData.camera = &data->camera;
  data->forwardData.targetTime = forwardDuration;
  data->forwardData.currentTime = data->timeForwardAnim;
  data->forwardData.oldValue = data->oldCoord;
  data->forwardData.sum = data->coordSum;

  data->npcTab = npcInitiate();
}

void update_dungeon(window *w) {
  dungeon_data *data = (dungeon_data *)w->window_data;
  Vector3 nextPosition = Vector3Add(
      Vector3Scale(GetCameraForward(&data->camera), PLAYER_MOVE_RANGE),
      data->camera.position);
  int checkX = nextPosition.x / PLAYER_MOVE_RANGE;
  int checkZ = nextPosition.z / PLAYER_MOVE_RANGE;

  bool stateSwitch = true;

  for (int i = 0; i < data->npcNumber; i++) {
    if (isFacingNPC(data->npcTab[i], nextPosition)) {
      data->facingNpc = data->npcTab[i];
      data->isFacingNpc = true;
    }
  }

  const char *message = read_next_message(w);
  if (message != NULL && data->animState == NONE) {
    if (strcmp(message, "up") == 0) {
      if ((checkX >= 0 && checkX <= MAP_SIZE_X - 1) &&
          (checkZ >= 0 && checkZ <= MAP_SIZE_Y - 1)) {
        if (data->map[checkX][checkZ] == 1) {
          if (!data->isFacingNpc)
            data->forwardData.currentTime = 0;
        }
      }
    } else if (strcmp(message, "left") == 0) {
      data->turnData.currentTime = 0;
      data->gauche = true;
    } else if (strcmp(message, "right") == 0) {
      data->turnData.currentTime = 0;
      data->gauche = false;
    }
  }

  if (w->focused) {
    if (data->gameState == IN_GAME) {
      // Input
      //--------------------------------------------------------------------------------------
      if (IsKeyDown(KEY_LEFT) && data->animState == NONE) {
        data->turnData.currentTime = 0;
        data->gauche = true;
      } else if (IsKeyDown(KEY_RIGHT) && data->animState == NONE) {
        data->turnData.currentTime = 0;
        data->gauche = false;
      } else if (IsKeyDown(KEY_UP) && data->animState == NONE) {
        if ((checkX >= 0 && checkX <= MAP_SIZE_X - 1) &&
            (checkZ >= 0 && checkZ <= MAP_SIZE_Y - 1)) {
          if (data->map[checkX][checkZ] == 1) {
            if (!data->isFacingNpc)
              data->forwardData.currentTime = 0;
          }
        }
      } else if (IsKeyPressed(KEY_E) && data->isFacingNpc) {
        data->gameState = IN_MENU;
        stateSwitch = false;
      }
    }

    if (data->gameState == IN_MENU) {
      if (IsKeyPressed(KEY_E) && stateSwitch == true) {
        data->gameState = IN_GAME;
      }
    }
  }

  // Function calling
  //--------------------------------------------------------------------------------------
  checkTurn(&data->turnData, &data->animState, data->gauche);
  checkForward(&data->forwardData, &data->animState);
}

void render_dungeon(window *w) {
  dungeon_data *data = (dungeon_data *)w->window_data;
  ClearBackground(RAYWHITE);

  BeginMode3D(data->camera);

  for (int i = 0; i < MAP_SIZE_X; i++) {
    for (int j = 0; j < MAP_SIZE_Y; j++) {
      if (data->map[i][j] == 1) {
        Vector3 floorPosition = {(i * PLAYER_MOVE_RANGE), 0.0f,
                                 (j * PLAYER_MOVE_RANGE)};
        Vector3 ceilingPosition = {(i * PLAYER_MOVE_RANGE), PLAYER_MOVE_RANGE,
                                   (j * PLAYER_MOVE_RANGE)};

        DrawPlane(floorPosition,
                  (Vector2){PLAYER_MOVE_RANGE, PLAYER_MOVE_RANGE}, PINK);
        DrawCubeV(ceilingPosition,
                  (Vector3){PLAYER_MOVE_RANGE, 0.1f, PLAYER_MOVE_RANGE}, PINK);
      } else if (data->map[i][j] == 2) {
        Vector3 wallSize = {PLAYER_MOVE_RANGE, PLAYER_MOVE_RANGE,
                            PLAYER_MOVE_RANGE};
        Vector3 wallPosition = {i * PLAYER_MOVE_RANGE, wallSize.y / 2,
                                j * PLAYER_MOVE_RANGE};
        DrawCubeV(wallPosition, wallSize, GRAY);
      }
    }
  }

  // Draw player cube
  for (int i = 0; i < data->npcNumber; i++) {
    DrawBillboard(data->camera, data->npcTab[i].texture,
                  data->npcTab[i].position, 3.0f, WHITE);
  }

  EndMode3D();

  // Draw info boxes
  DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)",
                      data->camera.position.x, data->camera.position.y,
                      data->camera.position.z),
           610, 15, 10, BLACK);
  DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)",
                      data->camera.target.x, data->camera.target.y,
                      data->camera.target.z),
           610, 30, 10, BLACK);
  DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", data->camera.up.x,
                      data->camera.up.y, data->camera.up.z),
           610, 45, 10, BLACK);
  if (data->gameState == IN_MENU) {
    Vector2 boxSize = {screenWidth * 0.7f, 150};
    Vector2 boxPos = {(screenWidth - boxSize.x) / 2, screenHeight - boxSize.y};
    DrawRectangleGradientV(boxPos.x, boxPos.y, boxSize.x, boxSize.y, VIOLET,
                           BLACK);
    DrawRectangleLinesEx((Rectangle){boxPos.x, boxPos.y, boxSize.x, boxSize.y},
                         5, BLACK);
    DrawText(TextFormat("%s", data->facingNpc.name), boxPos.x + 10,
             boxPos.y + 10, 20, WHITE);
    DrawText(TextFormat("%s", data->facingNpc.testDialogue), boxPos.x + 10,
             boxPos.y + 35, 15, WHITE);
  }
}
