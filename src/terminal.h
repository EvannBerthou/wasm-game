#ifndef TERMINAL_H
#define TERMINAL_H

#include "raylib.h"
#include <stddef.h>

void init_terminal(Vector2 position, Vector2 size);
void update_terminal(void);
void render_terminal(void);
Vector2 print_terminal_line(const char *text, size_t line);

#endif
