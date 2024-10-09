#include "npc.h"
#include "stdlib.h"
#include "raylib.h"
#include "stdio.h"
#include "string.h"

bool isFacingNPC(Npc npc,Vector3 nextPosition){
    bool isFacingNpc = false;

    if(npc.position.x == nextPosition.x && npc.position.z == nextPosition.z){
            isFacingNpc = true; 
    }
    return isFacingNpc;
}

Npc* npcInitiate(){
    int numberOfNpc = 2;

    //Npc init
    Npc *npcTab = (Npc*)malloc(sizeof(Npc)*numberOfNpc);

    Npc monoko;

    monoko.name = "Monoko\n";
    monoko.spritePath = "img/Monoko.png";
    monoko.texture = LoadTexture(monoko.spritePath); 
    monoko.position = (Vector3){15,1.5f,20};
    monoko.testDialogue = NULL;
    dialogueParser(&monoko);

    npcTab[0] = monoko;

    Npc jelly;

    jelly.name = "Jelly\n";
    jelly.spritePath = "img/Jelly.png";
    jelly.texture = LoadTexture(jelly.spritePath);
    jelly.position = (Vector3){20,1.5f,10};
    jelly.testDialogue = NULL;
    dialogueParser(&jelly);

    npcTab[1] = jelly; 
    
    return npcTab;
}
void dialogueParser(Npc *npc){
    FILE *dialogueFile = fopen("data/dialogue/dialogue.txt","rb"); 

    char* currentLine = malloc(sizeof(*dialogueFile));
    char* finalLine = malloc(sizeof(*dialogueFile));

    fgets(currentLine,100,dialogueFile); 

    while(strcmp(currentLine,npc->name) !=0){
        fgets(currentLine,100,dialogueFile); 
    }

    fgets(currentLine,100,dialogueFile); 

    while(strcmp(currentLine,"}\n")!=0){
        fgets(currentLine,100,dialogueFile); 
        if(strcmp(currentLine,"}\n")!=0){
            strcat(finalLine, currentLine);
            printf("'%s'\n", finalLine);
        }
    }

    npc->testDialogue = finalLine;
    fclose(dialogueFile);
}
