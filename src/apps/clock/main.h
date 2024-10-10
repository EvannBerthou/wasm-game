#ifndef CLOCK_H
#define CLOCK_H

#include "window.h"

window new_clock(int posx, int posy, int sizex, int sizey, const char *title);
void render_clock(window *w);

#endif
