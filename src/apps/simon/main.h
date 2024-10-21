#ifndef SIMON_H
#define SIMON_H

#include "window.h"

window new_simon(int posx, int posy, int sizex, int sizey, const char *title);
void render_simon(window *w);

#endif
