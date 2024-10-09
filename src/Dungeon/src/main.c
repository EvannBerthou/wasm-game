#include "npc.h"
#include "player_control.h"
#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "stddef.h"
#include "../../window.h"
#include <stdio.h>

#define MAP_SIZE_X 5
#define MAP_SIZE_Y 5

#define PLAYER_MOVE_RANGE 5
#define TURN_ANGLE 1.5708f

enum gameState { IN_GAME, IN_MENU };
enum gameState gameState = IN_GAME;
const int screenWidth = 960;
const int screenHeight = 540;

enum animState animState = NONE;
const float turnDuration = 0.3;
float timeTurnAnim = turnDuration;

float oldAngle = 0;
float rotateSum = 0;

bool gauche = false;

const float forwardDuration = 0.3;
float timeForwardAnim = forwardDuration;
float oldCoord = 0;
float coordSum = 0;
Camera3D camera = {0};

int map[MAP_SIZE_X][MAP_SIZE_Y] = {{1, 1, 1, 2, 1}, // 1=floor
                                   {1, 2, 1, 2, 1}, // 2=wall
                                   {1, 1, 1, 2, 1},
                                   {2, 1, 1, 1, 1},
                                   {2, 2, 1, 2, 2}};

AnimData turnData;
AnimData forwardData;

int npcNumber = 2;
Npc *npcTab;
bool isFacingNpc = false;
Npc facingNpc;

int init_dungeon(window *w) {
  camera.position = (Vector3){0.0f, 2.0f, 0.0f}; // Camera position
  camera.target = (Vector3){0.0f, 2.0f, 1.0f};   // Camera looking at point
  camera.up =
      (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
  camera.fovy = 90.0f;             // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

  turnData.camera = &camera;
  turnData.targetTime = turnDuration;
  turnData.currentTime = timeTurnAnim;
  turnData.oldValue = oldAngle;
  turnData.sum = rotateSum;

  forwardData.camera = &camera;
  forwardData.targetTime = forwardDuration;
  forwardData.currentTime = timeForwardAnim;
  forwardData.oldValue = oldCoord;
  forwardData.sum = coordSum;

  npcTab = npcInitiate();
}

void update_dungeon(window *w) {
  Vector3 nextPosition =
      Vector3Add(Vector3Scale(GetCameraForward(&camera), PLAYER_MOVE_RANGE),
                 camera.position);
  int checkX = nextPosition.x / PLAYER_MOVE_RANGE;
  int checkZ = nextPosition.z / PLAYER_MOVE_RANGE;

  bool stateSwitch = true;

  for (int i = 0; i < npcNumber; i++) {
    if (isFacingNPC(npcTab[i], nextPosition)) {
      facingNpc = npcTab[i];
      isFacingNpc = true;
    }
  }

  if (gameState == IN_GAME) {
    // Input
    //--------------------------------------------------------------------------------------
    if (IsKeyDown(KEY_LEFT) && animState == NONE) {
      turnData.currentTime = 0;
      gauche = true;
    } else if (IsKeyDown(KEY_RIGHT) && animState == NONE) {
      turnData.currentTime = 0;
      gauche = false;
    } else if (IsKeyDown(KEY_UP) && animState == NONE) {
      if ((checkX >= 0 && checkX <= MAP_SIZE_X - 1) &&
          (checkZ >= 0 && checkZ <= MAP_SIZE_Y - 1)) {
        if (map[checkX][checkZ] == 1) {
          if (!isFacingNpc)
            forwardData.currentTime = 0;
        }
      }
    } else if (IsKeyPressed(KEY_E) && isFacingNpc) {
      gameState = IN_MENU;
      stateSwitch = false;
    }

    // Function calling
    //--------------------------------------------------------------------------------------
    checkTurn(&turnData, &animState, gauche);
    checkForward(&forwardData, &animState);
  }

  if (gameState == IN_MENU) {
    if (IsKeyPressed(KEY_E) && stateSwitch == true) {
      gameState = IN_GAME;
    }
  }
}

void render_dungeon(window *w) {
  ClearBackground(RAYWHITE);

  BeginMode3D(camera);

  for (int i = 0; i < MAP_SIZE_X; i++) {
    for (int j = 0; j < MAP_SIZE_Y; j++) {
      if (map[i][j] == 1) {
        Vector3 floorPosition = {(i * PLAYER_MOVE_RANGE), 0.0f,
                                 (j * PLAYER_MOVE_RANGE)};
        Vector3 ceilingPosition = {(i * PLAYER_MOVE_RANGE), PLAYER_MOVE_RANGE,
                                   (j * PLAYER_MOVE_RANGE)};

        DrawPlane(floorPosition,
                  (Vector2){PLAYER_MOVE_RANGE, PLAYER_MOVE_RANGE}, PINK);
        DrawCubeV(ceilingPosition,
                  (Vector3){PLAYER_MOVE_RANGE, 0.1f, PLAYER_MOVE_RANGE}, PINK);
      } else if (map[i][j] == 2) {
        Vector3 wallSize = {PLAYER_MOVE_RANGE, PLAYER_MOVE_RANGE,
                            PLAYER_MOVE_RANGE};
        Vector3 wallPosition = {i * PLAYER_MOVE_RANGE, wallSize.y / 2,
                                j * PLAYER_MOVE_RANGE};
        DrawCubeV(wallPosition, wallSize, GRAY);
      }
    }
  }

  // Draw player cube
  for (int i = 0; i < npcNumber; i++) {
    DrawBillboard(camera, npcTab[i].texture, npcTab[i].position, 3.0f, WHITE);
  }

  EndMode3D();

  // Draw info boxes
  DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x,
                      camera.position.y, camera.position.z),
           610, 15, 10, BLACK);
  DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x,
                      camera.target.y, camera.target.z),
           610, 30, 10, BLACK);
  DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x,
                      camera.up.y, camera.up.z),
           610, 45, 10, BLACK);
  if (gameState == IN_MENU) {
    Vector2 boxSize = {screenWidth * 0.7f, 150};
    Vector2 boxPos = {(screenWidth - boxSize.x) / 2, screenHeight - boxSize.y};
    DrawRectangleGradientV(boxPos.x, boxPos.y, boxSize.x, boxSize.y, VIOLET,
                           BLACK);
    DrawRectangleLinesEx((Rectangle){boxPos.x, boxPos.y, boxSize.x, boxSize.y},
                         5, BLACK);
    DrawText(TextFormat("%s", facingNpc.name), boxPos.x + 10, boxPos.y + 10, 20,
             WHITE);
    DrawText(TextFormat("%s", facingNpc.testDialogue), boxPos.x + 10,
             boxPos.y + 35, 15, WHITE);
  }
}
