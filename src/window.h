#ifndef WINDOW_H
#define WINDOW_H

#include "raylib.h"
#include <stdint.h>

#define DEFAULT_PADDING 5.f
#define PAD(x) (x) + DEFAULT_PADDING
// Inline Padding Vector
#define INPADV(v) ((v.x) + DEFAULT_PADDING), ((v.y) + DEFAULT_PADDING)

typedef struct window {
    Vector2 pos;
    Vector2 size;
    char title[32];

    void (*update)(struct window*);
    void (*render)(struct window*);

    Vector2 origin;
} window;


void update_window(window *w, uint8_t id);
void render_window(window *w);

Rectangle get_window_inner(window *w);

#endif
