#ifndef TERMINAL_H
#define TERMINAL_H

#include "window.h"
#include <stddef.h>

#define MAX_INPUT_LINES 32
#define MAX_INPUT_LENGTH 255

typedef struct terminal_data {
    char input[MAX_INPUT_LINES][MAX_INPUT_LENGTH];
    size_t input_count;
    size_t input_length;
} terminal_data;

window new_terminal(int posx, int posy, int sizex, int sizey,
                    const char *title);
void init_terminal(window *w);
void update_terminal(window *w);
void render_terminal(window *w);

#endif
