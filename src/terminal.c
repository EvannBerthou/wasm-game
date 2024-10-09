#include "terminal.h"
#include "raylib.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_terminal(window *w) {
  terminal_data *data = malloc(sizeof(terminal_data));
  memset(data->input, 0, MAX_INPUT_LENGTH * MAX_INPUT_LINES);
  data->input_length = 0;
  data->input_count = 0;
  w->window_data = data;
}

void update_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");

  if (w->focused) {
    int key;
    do {
      if (data->input_length == MAX_INPUT_LENGTH)
        continue;
      // TODO: Handle non ASCII
      if ((key < 33 || key > 126) && key != KEY_SPACE)
        continue;
      data->input[data->input_count][data->input_length] = key;
      data->input_length++;
    } while ((key = GetCharPressed()) > 0);

    if (KeyPressed(KEY_BACKSPACE) && data->input_length > 0) {
      data->input_length--;
      data->input[data->input_count][data->input_length] = '\0';
    }

    if (KeyPressed(KEY_ENTER) && data->input_count < MAX_INPUT_LINES - 1) {
      data->input_count++;
      data->input_length = 0;
      data->input[data->input_count][data->input_length] = '\0';
    }
  }
}

// TODO: Add scrolling
void render_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");
  // Render Content
  Vector2 end_position;
  for (int i = 0; i <= data->input_count; i++) {
    end_position = print_terminal_line(w, data->input[i], i);
  }

  // Render Cursor
  DrawRectangleV(end_position, (Vector2){5, 18}, WHITE);


  //Debug rendering
  DrawCircle(w->size.x - 10, w->size.y - 10, 10, BLUE);
}

Vector2 print_terminal_line(window *w, const char *text, size_t line) {
  int y = 18 * line;
  Vector2 line_position = {0, y};
  DrawText(text, INPADV(line_position), 18, WHITE);
  int end_x = line_position.x + MeasureText(text, 18) + DEFAULT_PADDING * 2;
  int end_y = line_position.y + DEFAULT_PADDING;
  return (Vector2){end_x, end_y};
}
