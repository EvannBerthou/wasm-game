#ifndef DUNGEON_H
#define DUNGEON_H

#include "npc.h"
#include "player_control.h"
#include "window.h"

#define MAP_SIZE_X 5
#define MAP_SIZE_Y 5

#define PLAYER_MOVE_RANGE 5
#define TURN_ANGLE 1.5708f

enum gameState { IN_GAME, IN_MENU };

typedef struct dungeon_data {
  enum gameState gameState;
  enum animState animState;
  float timeTurnAnim;
  float oldAngle;
  float rotateSum;
  bool gauche;
  float timeForwardAnim;
  float oldCoord;
  float coordSum;
  Camera3D camera;
  int map[MAP_SIZE_X][MAP_SIZE_Y];
  AnimData turnData;
  AnimData forwardData;
  int npcNumber;
  Npc *npcTab;
  bool isFacingNpc;
  Npc facingNpc;
} dungeon_data;

window new_dungeon(int posx, int posy, int sizex, int sizey, const char *title);
void init_dungeon(window *w);
void update_dungeon(window *w);
void render_dungeon(window *w);

#endif
