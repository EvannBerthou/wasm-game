#include "terminal.h"
#include "raylib.h"
#include <assert.h>

#define MAX_INPUT_LINES 32
#define MAX_INPUT_LENGTH 255

// TODO: Remove shared buffer between all terminal instances.
char INPUTS[MAX_INPUT_LINES][MAX_INPUT_LENGTH] = {0};
size_t input_count = 0;
size_t input_length = 0;

void update_terminal(window *w) {
  int key;
  do {
    if (input_length == MAX_INPUT_LENGTH)
      continue;
    // TODO: Handle non ASCII
    if ((key < 33 || key > 126) && key != KEY_SPACE)
      continue;
    INPUTS[input_count][input_length] = key;
    input_length++;
  } while ((key = GetCharPressed()) > 0);

  // TODO: Handle repeating
  if (IsKeyPressed(KEY_BACKSPACE) && input_length > 0) {
    input_length--;
    INPUTS[input_count][input_length] = '\0';
  }

  if (IsKeyPressed(KEY_ENTER) && input_count < MAX_INPUT_LINES - 1) {
    input_count++;
    input_length = 0;
    INPUTS[input_count][input_length] = '\0';
  }
}

// TODO: Add scrolling
void render_terminal(window *w) {
  // Render Content
  Vector2 end_position;
  for (int i = 0; i <= input_count; i++) {
    end_position = print_terminal_line(w, INPUTS[i], i);
  }

  // Render Cursor
  DrawRectangleV(end_position, (Vector2){5, 18}, WHITE);
}

Vector2 print_terminal_line(window *w, const char *text, size_t line) {
  Rectangle inner = get_window_inner(w);
  int y = inner.y + 18 * line;
  Vector2 line_position = {inner.x, y};
  DrawText(text, INPADV(line_position), 18, WHITE);
  int end_x = line_position.x + MeasureText(text, 18) + DEFAULT_PADDING * 2;
  int end_y = line_position.y + DEFAULT_PADDING;
  return (Vector2){end_x, end_y};
}
