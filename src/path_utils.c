#include "path_utils.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool sshell_find_in_path(const char *command, char *out_path,
                         size_t out_path_size)
{
  if (command == NULL || out_path == NULL || out_path_size == 0) {
    return false;
  }

  char *path_env = getenv("PATH");
  if (path_env == NULL) {
    return false;
  }

  char *path_copy = strdup(path_env);
  if (path_copy == NULL) {
    return false;
  }

  bool found = false;
  char *dir = strtok(path_copy, ":");

  while (dir != NULL) {
    // Build the candidate path directly into the output buffer
    int written = snprintf(out_path, out_path_size, "%s/%s", dir, command);
    if (written > 0 && (size_t) written < out_path_size &&
        access(out_path, X_OK) == 0) {
      found = true;
      break;
    }

    dir = strtok(NULL, ":");
  }

  free(path_copy);
  return found;
}
