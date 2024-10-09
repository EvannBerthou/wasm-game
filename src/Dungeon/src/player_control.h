#ifndef PLAYER_CONTROL_H
#define PLAYER_CONTROL_H
#include "raylib.h"

#define PLAYER_MOVE_RANGE 5
#define TURN_ANGLE 1.5708f

enum animState {NONE,FORWARD,TURN};

typedef struct{
	Camera3D *camera;
	float targetTime;
	float currentTime;
	float oldValue;
	float sum;
}AnimData;

void checkTurn(AnimData *animData,enum animState *animState,bool gauche);
void checkForward(AnimData *animData,enum animState *animState);

#endif
