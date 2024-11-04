#include "main.h"
#include "utils.h"
#include "window.h"
#include <assert.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

window new_notepad(int posx, int posy, int sizex, int sizey,
                   const char *title) {
  window w = {.init = init_notepad,
              .update = update_notepad,
              .render = render_notepad,
              .free = free_notepad};
  init_window(&w, (Vector2){posx, posy}, (Vector2){sizex, sizey}, title);
  w.window_data = malloc(sizeof(notepad_data));
  assert(w.window_data != NULL);
  return w;
}

void init_notepad(window *w) {
  notepad_data *data = (notepad_data *)w->window_data;
  for (int i = 0; i < 10; i++) {
    memset(data->lines[i], 0, 32);
  }
  data->row_idx = 0;
  data->col_idx = 0;
}

void update_notepad(window *w) {
  notepad_data *data = (notepad_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");

  if (w->focused) {
    int key;
    do {
      if (data->col_idx == 31)
        continue;
      // TODO: Handle non ASCII
      if ((key < 33 || key > 126) && key != KEY_SPACE)
        continue;
      data->lines[data->row_idx][data->col_idx] = key;
      data->col_idx++;
    } while ((key = GetCharPressed()) > 0);

    if (KeyPressed(KEY_BACKSPACE)) {
      if (data->col_idx == 0 && data->row_idx == 0) {
        // Skip
      } else if (data->col_idx == 0 && data->row_idx > 0) {
        data->row_idx--;
        data->col_idx = strlen(data->lines[data->row_idx]);
      } else {
        data->col_idx--;
        // We already at the end of the string
        if ((data->lines[data->row_idx][data->col_idx + 1]) == '\0') {
          data->lines[data->row_idx][data->col_idx] = '\0';
        } else {
          char *line = data->lines[data->row_idx];
          memmove(&line[data->col_idx], &line[data->col_idx + 1],
                  strlen(line) - data->col_idx);
        }
      }
    }

    if (KeyPressed(KEY_UP) && data->row_idx > 0) {
      data->row_idx--;
      data->col_idx = strlen(data->lines[data->row_idx]);
    }

    if ((KeyPressed(KEY_ENTER) || KeyPressed(KEY_DOWN)) && data->row_idx < 9) {
      data->row_idx++;
      data->col_idx = strlen(data->lines[data->row_idx]);
    }

    if (KeyPressed(KEY_RIGHT) && data->col_idx < 31 &&
        data->col_idx < strlen(data->lines[data->row_idx])) {
      data->col_idx++;
    }

    if (KeyPressed(KEY_LEFT) && data->col_idx > 0) {
      data->col_idx--;
    }

    if (KeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_ALT) &&
        IsKeyDown(KEY_LEFT_CONTROL)) {
      const char *filename = data->lines[0];
      if (*filename == '\0') {
        printf("Please give a filename with the first line of the document.\n");
      } else {
        printf("Saving file with name : '%s'\n", filename);
        FILE *f = fopen(get_file_path(filename), "w");
        if (f == NULL) {
          printf("Error opening file.\n");
          exit(0); // TODO: Handling
        }
        for (size_t i = 1; i < 10; i++) {
          fwrite(data->lines[i], sizeof(char), strlen(data->lines[i]), f);
          fwrite("\n", sizeof(char), 1, f);
        }
        fclose(f);
        printf("File saved!\n");
      }
    }
  }
}

void render_notepad(window *w) {
  notepad_data *data = (notepad_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");

  for (size_t i = 0; i < 10; i++) {
    const char *text = data->lines[i];
    Vector2 line_position = {DEFAULT_PADDING, 20 * i + DEFAULT_PADDING};
    DrawTextEx(GetDefaultFont(), text, line_position, 20, 3, WHITE);

    if (i == data->row_idx) {
      char text_up_to_cursor[32] = {0};
      strncpy(text_up_to_cursor, text, data->col_idx);
      int cursor_x =
          MeasureTextEx(GetDefaultFont(), text_up_to_cursor, 20, 3).x;
      Vector2 cursor = {.x = cursor_x + DEFAULT_PADDING,
                        .y = data->row_idx * 20 + DEFAULT_PADDING};
      DrawRectangleV(cursor, (Vector2){3, 18}, WHITE);
    }
  }
}

void free_notepad(window *w) { (void)w; }
