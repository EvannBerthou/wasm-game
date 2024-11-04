#ifndef NOTEPAD_H
#define NOTEPAD_H

#include "window.h"

typedef struct notepad_data {
    char lines[10][32];
    size_t row_idx;
    size_t col_idx;
} notepad_data;

window new_notepad(int posx, int posy, int sizex, int sizey,
                    const char *title);
void init_notepad(window *w);
void update_notepad(window *w);
void render_notepad(window *w);
void free_notepad(window *w);

#endif
