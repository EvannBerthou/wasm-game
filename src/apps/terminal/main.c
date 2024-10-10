#include "main.h"
#include "../../utils.h"
#include "raylib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

window new_terminal(int posx, int posy, int sizex, int sizey,
                    const char *title) {
  window w = {.init = init_terminal,
              .update = update_terminal,
              .render = render_terminal};
  init_window(&w, (Vector2){posx, posy}, (Vector2){sizex, sizey}, title);
  w.window_data = malloc(sizeof(terminal_data));
  assert(w.window_data != NULL);
  return w;
}

void init_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  memset(data->input, 0, sizeof(data->input));
  data->input_length = 0;
  data->input_count = 0;
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

static Vector2 print_terminal_line(const char *text, size_t line) {
  int y = 18 * line;
  Vector2 line_position = {0, y};
  DrawText(text, INPADV(line_position), 18, WHITE);
  int end_x = line_position.x + MeasureText(text, 18) + DEFAULT_PADDING * 2;
  int end_y = line_position.y + DEFAULT_PADDING;
  return (Vector2){end_x, end_y};
}

// TODO: Add scrolling
void render_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");

  // Render Content
  Vector2 end_position;
  for (size_t i = 0; i <= data->input_count; i++) {
    end_position = print_terminal_line(data->input[i], i);
  }

  // Render Cursor
  DrawRectangleV(end_position, (Vector2){5, 18}, WHITE);

  // Debug rendering
  DrawCircle(w->size.x - 10, w->size.y - 10, 10, BLUE);
}
