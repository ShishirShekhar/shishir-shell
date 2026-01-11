#define _POSIX_C_SOURCE 200809L

#include "executor.h"

#include "path_utils.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

bool sshell_execute_external(const char *command, char *argv[])
{
  char resolved_path[PATH_MAX];

  if (!sshell_find_in_path(command, resolved_path, sizeof(resolved_path))) {
    return false;
  }

  pid_t pid = fork();
  if (pid == 0) {
    execv(resolved_path, argv);
    perror("execv failed");
    _exit(EXIT_FAILURE);
  } else if (pid > 0) {
    int status = 0;
    waitpid(pid, &status, 0);
    return true;
  }

  return false;
}
