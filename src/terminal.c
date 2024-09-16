#include "terminal.h"
#include "raylib.h"
#include "raymath.h"
#include <assert.h>
#include <stdio.h>

#define DEFAULT_PADDING 5.f
#define PAD(x) (x) + DEFAULT_PADDING
// Inline Padding Vector
#define INPADV(v) ((v.x) + DEFAULT_PADDING), ((v.y) + DEFAULT_PADDING)

Vector2 terminal_size;
Vector2 terminal_position;
Rectangle inner_terminal;
Rectangle outer_terminal;

// Left, Top, Right, Bottom
Vector4 paddings = {DEFAULT_PADDING, 30, DEFAULT_PADDING, DEFAULT_PADDING};

void init_terminal(Vector2 position, Vector2 size) {
  terminal_position = position;
  terminal_size = size;

  inner_terminal = (Rectangle){terminal_position.x + paddings.x,
                               terminal_position.y + paddings.y,
                               terminal_size.x - paddings.x - paddings.z,
                               terminal_size.y - paddings.y - paddings.w};
}

#define MAX_INPUT_LINES 32
#define MAX_INPUT_LENGTH 255

char INPUTS[MAX_INPUT_LINES][MAX_INPUT_LENGTH] = {0};
size_t input_count = 0;
size_t input_length = 0;

int dragging = 0;
Vector2 origin = {0};

void update_terminal(void) {
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


  // TODO: Should be handled by desktop.c
  Rectangle header = {terminal_position.x, terminal_position.y, terminal_size.x,
                      paddings.y};

  int hovering = CheckCollisionPointRec(GetMousePosition(), header);
  if (hovering && IsMouseButtonPressed(0)) {
      dragging = 1;
      origin = Vector2Subtract(GetMousePosition(), terminal_position);
  }

  if (IsMouseButtonReleased(0)) {
      dragging = 0;
      origin = Vector2Zero();
  }

  if (dragging) {
      Vector2 new_position = Vector2Subtract(GetMousePosition(), origin);
      init_terminal(new_position, terminal_size);
  }
}

void render_terminal(void) {
  assert(terminal_size.x != 0 && terminal_size.y != 0 &&
         "Call init_terminal before rendering it.");

  DrawRectangleV(terminal_position, terminal_size, LIGHTGRAY);
  DrawRectangleRec(inner_terminal, BLACK);

  // Render terminal title
  DrawText("Terminal", INPADV(terminal_position), 20, BLACK);

  // Render Content
  Vector2 end_position;
  for (int i = 0; i <= input_count; i++) {
    end_position = print_terminal_line(INPUTS[i], i);
  }

  // Render Cursor
  DrawRectangleV(end_position, (Vector2){5, 18}, WHITE);
}

Vector2 print_terminal_line(const char *text, size_t line) {
  int y = inner_terminal.y + 18 * line;
  Vector2 line_position = {inner_terminal.x, y};
  DrawText(text, INPADV(line_position), 18, WHITE);
  return (Vector2){line_position.x + MeasureText(text, 18) +
                       DEFAULT_PADDING * 2,
                   line_position.y + DEFAULT_PADDING};
}
