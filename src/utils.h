#ifndef UTIL_H
#define UTIL_H

#include "raylib.h"

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)

static inline bool KeyPressed(int key) {
  return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

void SetDefaultFont(Font font);
Font GetDefaultFont(void);

#endif
