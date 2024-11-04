#include "main.h"
#include "../../utils.h"
#include "desktop.h"
#include "raylib.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

window new_terminal(int posx, int posy, int sizex, int sizey,
                    const char *title) {
  window w = {.init = init_terminal,
              .update = update_terminal,
              .render = render_terminal,
              .free = free_terminal};
  init_window(&w, (Vector2){posx, posy}, (Vector2){sizex, sizey}, title);
  w.window_data = malloc(sizeof(terminal_data));
  assert(w.window_data != NULL);
  return w;
}

static terminal_line *new_terminal_line(char *content, terminal_line *prev) {
  terminal_line *line = malloc(sizeof(terminal_line));
  strncpy(line->content, content, MAX_INPUT_LENGTH);
  memset(line->prompt, 0, MAX_INPUT_LENGTH);
  line->prev = prev;
  return line;
}

static void append_line(terminal_data *data, char *content) {
  terminal_line *line = new_terminal_line(content, data->commands);
  data->commands = line;
  data->line_count++;
}

static void prepand_prompt(terminal_data *data) {
  char *prompt = "$ ";
  strncpy(data->commands->prompt, prompt, MAX_INPUT_LENGTH);
}

static void new_line(terminal_data *data) {
  append_line(data, "");
  prepand_prompt(data);
}

static char *trim(char *base) {
  while (isspace(*base))
    base++;

  char *end = base + strlen(base);
  while (isspace(*--end))
    ;
  *(end + 1) = '\0';
  return base;
}

static char *shift_args(const char *cmd) {
  char *args = strchr(cmd, ' ');
  if (args != NULL) {
    while (*args != '\0' && *args == ' ') {
      args++;
    }
  }
  return args;
}

void init_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  terminal_line *tail = new_terminal_line("Type help!", NULL);
  data->commands = tail;

  data->input_length = 0;
  data->line_count = 0;
  pthread_mutex_init(&data->cmd_mutex, NULL);
  data->tid = 0;

  new_line(data);
  prepand_prompt(data);
}

void update_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");

  // if there is a thread going on
  if (data->tid != 0) {
    return;
  }

  if (w->focused) {
    int key;
    do {
      if (data->input_length == MAX_INPUT_LENGTH)
        continue;
      // TODO: Handle non ASCII
      if ((key < 33 || key > 126) && key != KEY_SPACE)
        continue;
      data->commands->content[data->input_length] = key;
      data->input_length++;
    } while ((key = GetCharPressed()) > 0);

    if (KeyPressed(KEY_BACKSPACE) && data->input_length > 0) {
      data->input_length--;
      data->commands->content[data->input_length] = '\0';
    }

    if (KeyPressed(KEY_ENTER)) {
      handle_command(data);
      data->input_length = 0;
    }
  }
}

const size_t FONT_SIZE = 20;
static Vector2 print_terminal_line(const char *text, size_t line,
                                   size_t offset) {
  int y = FONT_SIZE * line;
  Vector2 line_position = {offset + DEFAULT_PADDING, y + DEFAULT_PADDING};
  DrawTextEx(GetDefaultFont(), text, line_position, FONT_SIZE, 3, WHITE);
  Vector2 text_size = MeasureTextEx(GetDefaultFont(), text, FONT_SIZE, 3);
  int end_x = line_position.x + text_size.x;
  int end_y = line_position.y;
  return (Vector2){end_x, end_y};
}

// TODO: Handle line where length is bigger than window width
void render_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");

  pthread_mutex_lock(&data->cmd_mutex);
  {
    int window_height = w->size.y - DEFAULT_PADDING;
    int max_line_count = floor((double)window_height / FONT_SIZE - 1);
    int line_count = MIN((int)data->line_count, max_line_count);

    // Render Content
    Vector2 end_position = Vector2Zero();
    int i = line_count;
    for (terminal_line *line = data->commands; line != NULL && i >= 0;
         line = line->prev, i--) {
      Vector2 prompt_pos = print_terminal_line(line->prompt, i, 0);
      Vector2 pos = print_terminal_line(line->content, i, prompt_pos.x);
      if (i == line_count) {
        end_position = pos;
      }
    }

    // Render Cursor only if not processing a command
    if (data->tid == 0) {
      DrawRectangleV(end_position, (Vector2){3, 18}, WHITE);
    }
  }
  pthread_mutex_unlock(&data->cmd_mutex);
}

void free_terminal(window *w) {
  terminal_data *data = (terminal_data *)w->window_data;
  assert(data != NULL && "Data should never be NULL");

  terminal_line *head = data->commands;
  while (head != NULL) {
    terminal_line *prev = head->prev;
    head->prev = NULL;
    free(head);
    head = prev;
  }
  data->commands = NULL;
}

void console_log(terminal_data *data, const char *log) {
  pthread_mutex_lock(&data->cmd_mutex);
  {
    char *line = strtok((char *)log, "\n");
    while (line != NULL) {
      append_line(data, line);
      line = strtok(NULL, "\n");
    }
  }
  pthread_mutex_unlock(&data->cmd_mutex);
}

static void thread_end_callback(terminal_thread *tt) {
  tt->data->tid = 0;
  new_line(tt->data);
  free((void *)tt->args);
  free(tt);
}

// TODO: Threads create race conditions on TextFormat. Sometime, a TextFormat
// will be replaced by a "60 FPS". TextFormat is not thread safe
static void call_cmd(terminal_data *data, const char *cmd, cmd_func func) {
  char *args = shift_args(cmd);
  terminal_thread *tt = malloc(sizeof(terminal_thread));
  tt->data = data;
  tt->args = strdup(args);
  tt->callback = &thread_end_callback;
  int err = pthread_create(&data->tid, NULL, func, (void *)tt);
  if (err) {
    console_log(data, TextFormat("Error starting thread: '%s'", strerror(err)));
    data->tid = 0;
    new_line(data);
    free(tt);
  }
}

BEGIN_CMD(echo) {
  if (args != NULL) {
    console_log(data, trim((char *)args));
  }
}
END_CMD()

BEGIN_CMD(sleep) {
  if (args == NULL) {
    console_log(data, "sleep <seconds>");
    ret(0);
  }
  int seconds = 0;
  if (!strtoint(args, &seconds)) {
    console_log(data, "sleep <seconds>");
    ret(0);
  }
  sleep(seconds);
}
END_CMD()

BEGIN_CMD(help) {
  console_log(data,
              "echo <text>, sleep <seconds>, help, ps, send <pid> <text>");
}
END_CMD()

BEGIN_CMD(ps) {
  desktop *d = get_desktop();
  for (size_t i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (d->windows[i].id != DISABLED_WINDOW) {
      console_log(data, TextFormat("ID=%d TITLE=%s\n", d->windows[i].id,
                                   d->windows[i].title));
    }
  }
}
END_CMD()

BEGIN_CMD(send) {
  int part_count = 0;
  const char **parts = TextSplit(args, ' ', &part_count);
  if (part_count != 2) {
    console_log(data, "send <pid> <text>");
    ret(0);
  }
  int iwid = 0;
  if (!strtoint(parts[0], &iwid)) {
    console_log(data, "send <pid> <text>");
    ret(0);
  }

  uint32_t wid = iwid;
  desktop *d = get_desktop();
  window *w = NULL;
  for (size_t i = 0; i < MAX_WINDOWS_COUNT; i++) {
    if (d->windows[i].id == wid) {
      w = &d->windows[i];
      break;
    }
  }

  if (w == NULL) {
    console_log(data, TextFormat("No window with ID=%d", wid));
    ret(0);
  }

  console_log(data, TextFormat("Sending '%s' to ID=%d\n", parts[1], wid));
  send_window_message(w, parts[1]);
}
END_CMD()

const char *programs[][128] = {
    {"echo a", "help", "sleep 3", "ps", "send 0 left", "zibzoub", NULL},
    {"send 0 left", "sleep 1", "send 0 up", "sleep 1", "send 0 up", "sleep 1",
     "send 0 right", "sleep 1", "send 0 up", "sleep 1", "send 0 up", "sleep 1",
     "send 0 left", "sleep 1", "send 0 up", NULL},
    {"echo b", "echo c", NULL},
};

BEGIN_CMD(exec) {
  int program_id = 0;
  if (!strtoint(args, &program_id)) {
    console_log(data, TextFormat("exec <id>"));
    ret(0);
  }

  if (program_id >= 2) {
    console_log(data, "program_id needs to be between 0 and 1 (inclusive).");
    ret(0);
  }

  const char **program = programs[program_id];
  char local_cmd_buf[128] = {0};
  while (*program != NULL) {
    strncpy(local_cmd_buf, *program, 128);
    char *cmd_trimmed = trim(local_cmd_buf);
    cmd_func *f = get_command_function_by_name(cmd_trimmed);
    if (f != NULL) {
      const char *exec_args = shift_args(cmd_trimmed);
      console_log(data, TextFormat("> %s", cmd_trimmed));
      terminal_thread tt = {.data = data, .args = exec_args, .callback = NULL};
      f(&tt);
    } else {
      console_log(data, TextFormat("Unknown command '%s'", cmd_trimmed));
      new_line(data);
    }
    program++;
  }
}
END_CMD()

command commands[] = {COMMAND(echo), COMMAND(sleep), COMMAND(help),
                      COMMAND(ps),   COMMAND(send),  COMMAND(exec)};
size_t command_count = sizeof(commands) / sizeof(command);

void handle_command(terminal_data *data) {
  char *cmd_trimmed = trim(data->commands->content);
  if (*cmd_trimmed == '\0') {
    new_line(data);
    return;
  }

  cmd_func *f = get_command_function_by_name(cmd_trimmed);
  if (f != NULL) {
    call_cmd(data, cmd_trimmed, f);
  } else {
    console_log(data, TextFormat("Unknown command '%s'", cmd_trimmed));
    new_line(data);
  }
}

cmd_func *get_command_function_by_name(char *cmd) {
  for (size_t i = 0; i < command_count; i++) {
    if (PREFIX(commands[i].name, cmd)) {
      return commands[i].func;
    }
  }
  return NULL;
}
