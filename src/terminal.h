#ifndef TERMINAL_H
#define TERMINAL_H

#include "raylib.h"
#include "window.h"
#include <stddef.h>

void update_terminal(window *w);
void render_terminal(window *w);
Vector2 print_terminal_line(window *w, const char *text, size_t line);

#endif
