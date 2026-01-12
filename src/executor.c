#define _POSIX_C_SOURCE 200809L

#include "executor.h"

#include "path_utils.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

bool sshell_execute_external(const char *command, char *argv[],
                             const char *provided_out_file,
                             const char *provided_err_file, bool out_append,
                             bool err_append)
{
  char resolved_path[PATH_MAX];

  if (!sshell_find_in_path(command, resolved_path, sizeof(resolved_path))) {
    return false;
  }

  const char *out_file = provided_out_file;
  const char *err_file = provided_err_file;

  pid_t pid = fork();
  if (pid == 0) {
    if (out_file != NULL) {
      int flags = O_CREAT | O_WRONLY | (out_append ? O_APPEND : O_TRUNC);
      int fd = open(out_file, flags, 0644);
      if (fd < 0) {
        perror("open failed");
        _exit(EXIT_FAILURE);
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
    }

    if (err_file != NULL) {
      int flags = O_CREAT | O_WRONLY | (err_append ? O_APPEND : O_TRUNC);
      int fd = open(err_file, flags, 0644);
      if (fd < 0) {
        perror("open failed");
        _exit(EXIT_FAILURE);
      }
      dup2(fd, STDERR_FILENO);
      close(fd);
    }

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
