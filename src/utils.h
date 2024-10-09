#ifndef UTIL_H
#define UTIL_H

#include "raylib.h"
static inline bool KeyPressed(int key) {
  return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

#endif
