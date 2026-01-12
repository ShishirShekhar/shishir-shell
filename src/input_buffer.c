#define _POSIX_C_SOURCE 200809L

#include "input_buffer.h"

#include "builtin.h"

#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

InputBuffer Create_Input_Buffer()
{
  InputBuffer input_buffer;

  input_buffer.input = calloc(MAX_INPUT_BUFFER, sizeof(char));
  input_buffer.input_size = 0;

  return input_buffer;
}

// Tab completion generator for external commands in PATH
static char *external_command_generator(const char *text, int state)
{
  static DIR *dir = NULL;
  static char **path_dirs = NULL;
  static size_t path_index = 0;
  static size_t len;

  // Initialize on first call
  if (!state) {
    path_index = 0;
    len = strlen(text);
    dir = NULL;

    // Parse PATH into array of directories
    const char *path_env = getenv("PATH");
    if (path_env) {
      char *path_copy = strdup(path_env);
      // Count directories
      size_t count = 1;
      for (char *p = path_copy; *p; p++) {
        if (*p == ':')
          count++;
      }
      // Allocate array
      path_dirs = malloc((count + 1) * sizeof(char *));
      size_t i = 0;
      char *token = strtok(path_copy, ":");
      while (token) {
        path_dirs[i++] = strdup(token);
        token = strtok(NULL, ":");
      }
      path_dirs[i] = NULL;
      free(path_copy);
    }
  }

  // Search through PATH directories
  while (path_dirs && path_dirs[path_index]) {
    if (dir == NULL) {
      dir = opendir(path_dirs[path_index]);
      if (dir == NULL) {
        path_index++;
        continue;
      }
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      // Skip hidden files and check prefix match
      if (entry->d_name[0] == '.' || strncmp(entry->d_name, text, len) != 0) {
        continue;
      }

      // Check if executable
      char full_path[1024];
      snprintf(full_path, sizeof(full_path), "%s/%s", path_dirs[path_index],
               entry->d_name);
      if (access(full_path, X_OK) == 0) {
        return strdup(entry->d_name);
      }
    }

    closedir(dir);
    dir = NULL;
    path_index++;
  }

  // Cleanup
  if (path_dirs) {
    for (size_t i = 0; path_dirs[i]; i++) {
      free(path_dirs[i]);
    }
    free(path_dirs);
    path_dirs = NULL;
  }

  return NULL;
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

// Combined command generator: builtins first, then external commands
static char *command_generator(const char *text, int state)
{
  static int checking_builtins = 1;
  char *match = NULL;

  if (!state) {
    checking_builtins = 1;
  }

  // Try builtins first
  if (checking_builtins) {
    match = builtin_command_generator(text, state);
    if (match == NULL) {
      checking_builtins = 0;
      state = 0; // Reset state for external generator
    }
  }

  // Then try external commands
  if (!checking_builtins) {
    match = external_command_generator(text, state);
  }

  return match;
}

// Custom completion function
static char **command_completion(const char *text, int start,
                                 int end __attribute__((unused)))
{
  char **matches = NULL;

  // Only complete if at the beginning of the line (command position)
  if (start == 0) {
    matches = rl_completion_matches(text, command_generator);
  }

  return matches;
}

uint8_t capture_input(InputBuffer *input_buffer)
{
  // Set up readline completion (only once)
  static int completion_initialized = 0;
  if (!completion_initialized) {
    rl_attempted_completion_function = command_completion;
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
