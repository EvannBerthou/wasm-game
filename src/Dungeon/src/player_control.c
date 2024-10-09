#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "stddef.h"

#include "player_control.h"

void checkTurn(AnimData *animData,enum animState *animState,bool gauche){
    if(animData->currentTime < animData->targetTime){
        *animState = TURN;

        animData->currentTime +=GetFrameTime();

        float t = animData->currentTime / animData->targetTime;

        float newAngle = Lerp(0,TURN_ANGLE,t);
        float deltaAngle = newAngle - animData->oldValue;
        animData->oldValue = newAngle;
        CameraYaw(animData->camera,deltaAngle * (gauche ? 1 : -1),false);
        
        animData->sum += deltaAngle;

        //End of animation
        if(animData->currentTime>=animData->targetTime){
            CameraYaw(animData->camera,(TURN_ANGLE - animData->sum) * (gauche ? 1 : -1),false);
            animData->camera->target.x=roundf(animData->camera->target.x);
            animData->camera->target.y=roundf(animData->camera->target.y);
            animData->camera->target.z=roundf(animData->camera->target.z);
        }    
        
    }else if(*animState == TURN){
        animData->oldValue=0;
        animData->sum = 0;

        *animState = NONE;
    }
}
void checkForward(AnimData *animData,enum animState *animState){

    if(animData->currentTime < animData->targetTime){
        *animState = FORWARD;

        animData->currentTime += GetFrameTime();

        float t = animData->currentTime / animData->targetTime;
        float newCoord = Lerp(0,PLAYER_MOVE_RANGE,t);
        float deltaCoord = newCoord - animData->oldValue;
        animData->oldValue = newCoord;
        CameraMoveForward(animData->camera,deltaCoord,false);

        animData->sum += deltaCoord;
        
        //End of animation
        if(animData->currentTime>=animData->targetTime){
            CameraMoveForward(animData->camera,(PLAYER_MOVE_RANGE - animData->sum),false);
            animData->camera->position.x=roundf(animData->camera->position.x);
            animData->camera->position.y=roundf(animData->camera->position.y);
            animData->camera->position.z=roundf(animData->camera->position.z);
        } 
    }
    else if(*animState == FORWARD){
        animData->oldValue = 0;
        animData->sum = 0;

        *animState = NONE;
    }
}
