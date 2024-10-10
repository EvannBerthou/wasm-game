#ifndef DUNGEON_H
#define DUNGEON_H

#include "window.h"

window new_dungeon(int posx, int posy, int sizex, int sizey,
                   const char *title);
void init_dungeon(window *w);
void update_dungeon(window *w);
void render_dungeon(window *w);

#endif
