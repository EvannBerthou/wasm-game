#ifndef TERMINAL_H
#define TERMINAL_H

#include "window.h"
#include <pthread.h>
#include <stddef.h>

#define MAX_INPUT_LENGTH 255
#define MAX_PROMPT_LENGTH 32

typedef struct terminal_line {
  char content[MAX_INPUT_LENGTH];
  char prompt[MAX_INPUT_LENGTH];
  struct terminal_line *prev;
} terminal_line;

typedef struct terminal_data {
  terminal_line *commands;
  size_t input_length;
  size_t line_count;
  pthread_mutex_t cmd_mutex;
  pthread_t tid;
} terminal_data;

typedef struct terminal_thread {
  terminal_data *data;
  const char *args;
} terminal_thread;

window new_terminal(int posx, int posy, int sizex, int sizey,
                    const char *title);
void init_terminal(window *w);
void update_terminal(window *w);
void render_terminal(window *w);

void console_log(terminal_data *data, const char *log);
void handle_command(terminal_data *data);

#define PREFIX(pre, str) strncmp((pre), (str), strlen(pre)) == 0

#define BEGIN_CMD(name)                                                        \
  static void *cmd_##name(void *argsv) {                                       \
    terminal_thread *terminal_thread_args = (terminal_thread *)argsv;          \
    terminal_data *data = terminal_thread_args->data;                          \
    const char *args = terminal_thread_args->args;                             \
    void *ret_val = 0;                                                         \
    {

#define END_CMD()                                                              \
  }                                                                            \
  ret(0);                                                                      \
  end_ret:                                                                     \
  terminal_thread_args->data->tid = 0;                                         \
  new_line(data);                                                              \
  free(argsv);                                                                 \
  return (void *)ret_val;                                                      \
  }

#define ret(x)                                                                 \
  ret_val = (x);                                                               \
  goto end_ret

// Commands

typedef void *(cmd_func)(void *);

typedef struct command {
  const char *name;
  cmd_func *func;
} command;

#define COMMAND(cmd_name)                                                      \
  { .name = #cmd_name, .func = cmd_##cmd_name }

#endif
