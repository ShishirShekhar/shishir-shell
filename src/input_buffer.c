#include "input_buffer.h"

#include "builtin.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InputBuffer Create_Input_Buffer()
{
  InputBuffer input_buffer;

  input_buffer.input = calloc(MAX_INPUT_BUFFER, sizeof(char));
  input_buffer.input_size = 0;

  return input_buffer;
}

// Tab completion generator for builtin commands
static char *builtin_command_generator(const char *text, int state)
{
  static size_t list_index;
  static size_t len;
  static size_t builtin_count;

  // Initialize on first call
  if (!state) {
    list_index = 0;
    len = strlen(text);
    builtin_count = sshell_get_builtin_count();
  }

  // Return matching builtins
  while (list_index < builtin_count) {
    const char *name = sshell_get_builtin_name(list_index++);
    if (name && strncmp(name, text, len) == 0) {
      return strdup(name);
    }
  }

  return NULL;
}

// Custom completion function
static char **builtin_completion(const char *text, int start,
                                 int end __attribute__((unused)))
{
  char **matches = NULL;

  // Only complete if at the beginning of the line (command position)
  if (start == 0) {
    matches = rl_completion_matches(text, builtin_command_generator);
  }

  return matches;
}

uint8_t capture_input(InputBuffer *input_buffer)
{
  // Set up readline completion (only once)
  static int completion_initialized = 0;
  if (!completion_initialized) {
    rl_attempted_completion_function = builtin_completion;
    completion_initialized = 1;
  }

  // Read line with readline (provides history and completion)
  char *line = readline("sshell> ");

  // Handle EOF (Ctrl+D)
  if (line == NULL) {
    input_buffer->input[0] = '\0';
    input_buffer->input_size = 0;
    return 0;
  }

  // Copy to input buffer
  strncpy(input_buffer->input, line, MAX_INPUT_BUFFER - 1);
  input_buffer->input[MAX_INPUT_BUFFER - 1] = '\0';
  input_buffer->input_size = strlen(input_buffer->input);

  // Add to history if non-empty
  if (input_buffer->input_size > 0) {
    add_history(line);
  }

  free(line);
  return input_buffer->input_size;
}

void free_input_buffer(InputBuffer *input_buffer)
{
  free(input_buffer->input);
  input_buffer->input = NULL;
  input_buffer->input_size = 0;
}
